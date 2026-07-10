#include <switch.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAZE_W 15
#define MAZE_H 9

// 10 уровней. На 10-м уровне стоит цифра 4 (скример)
int levels[MAZE_H][MAZE_W] = {
    // Уровень 1
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,1,0,0,0,0,0,1,0,0,3,1},{1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},{1,0,1,1,1,1,1,0,1,1,1,1,1,0,1},{1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},{1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 2
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},{1,1,1,1,1,1,1,0,1,1,1,1,1,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1},{1,0,1,1,1,1,1,1,1,1,1,0,1,0,1},{1,0,0,0,0,0,0,0,0,0,1,0,0,3,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 3
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{1,1,1,1,1,0,1,0,1,1,1,1,1,1,1},{1,0,0,0,1,0,1,0,1,0,0,0,0,0,1},{1,0,1,0,1,0,0,0,1,0,1,1,1,0,1},{1,0,1,0,1,1,1,1,1,0,1,0,1,0,1},{1,0,1,0,0,0,0,0,0,0,1,0,1,0,1},{1,3,1,1,1,1,1,1,1,1,1,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 4
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},{1,0,1,0,1,1,1,0,1,0,1,1,1,1,1},{1,0,1,0,1,0,1,0,1,0,1,0,0,0,1},{1,0,0,0,1,0,1,0,0,0,1,0,1,0,1},{1,1,1,1,1,0,1,1,1,1,1,0,1,0,1},{1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},{1,3,1,1,1,1,1,1,1,1,1,1,1,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 5
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,3,1},{1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,0,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 6
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,1,0,0,0,1,0,0,0,1,0,0,0,1},{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},{1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},{1,1,1,1,1,1,1,0,1,1,1,1,1,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1},{1,0,1,1,1,1,1,1,1,1,1,0,1,0,1},{1,0,0,0,0,0,0,0,0,0,1,0,0,3,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 7
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,1,0,0,0,0,0,0,0,1},{1,1,1,1,1,0,1,0,1,1,1,1,1,1,1},{1,0,0,0,1,0,1,0,1,0,0,0,0,0,1},{1,0,1,0,1,0,0,0,1,0,1,1,1,0,1},{1,0,1,0,1,1,1,1,1,0,1,0,1,0,1},{1,0,1,0,0,0,0,0,0,0,1,0,1,0,1},{1,3,1,1,1,1,1,1,1,1,1,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 8
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},{1,0,1,0,1,1,1,0,1,0,1,1,1,1,1},{1,0,1,0,1,0,1,0,1,0,1,0,0,0,1},{1,0,0,0,1,0,1,0,0,0,1,0,1,0,1},{1,1,1,1,1,0,1,1,1,1,1,0,1,0,1},{1,0,0,0,0,0,1,0,0,0,0,0,1,0,1},{1,3,1,1,1,1,1,1,1,1,1,1,1,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // Уровень 9
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,1,0,0,0,0,0,1,0,0,3,1},{1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},{1,0,1,1,1,1,1,0,1,1,1,1,1,0,1},{1,0,0,0,1,0,0,0,0,0,1,0,0,0,1},{1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},{1,0,0,0,0,0,1,0,1,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}},
    // УРОВЕНЬ 10 - СКРИМЕР
    {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,0,1,1,1,1,1,0,1},{1,0,0,0,0,0,1,0,1,0,0,0,1,0,1},{1,0,1,1,1,0,1,0,1,0,1,0,1,0,1},{1,0,1,0,1,0,0,4,0,0,1,0,0,0,1},{1,0,1,0,1,1,1,1,1,1,1,1,1,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,3,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}}
};

// Генератор математического адского шума (без аудиофайлов)
void play_procedural_scream() {
    audoutInitialize();
    audoutStartAudioOut();
    
    const uint32_t sample_rate = 44100;
    const uint32_t duration_sec = 2; // Сколько секунд орет скример
    const uint32_t chunk_size = 4096 * 4;
    
    uint32_t total_samples = sample_rate * duration_sec * 2; // Стерео
    int16_t* audio_buffer = (int16_t*)malloc(total_samples * sizeof(int16_t));
    
    // Заполняем буфер жутким пугающим скрежетом
    for (uint32_t i = 0; i < total_samples; i += 2) {
        // Математический агрессивный скрежет
        int16_t noise = (rand() % 16384) - 8192;
        int16_t tone = (i % 44 < 22) ? 12000 : -12000; // Пронзительный писк
        
        audio_buffer[i] = noise + tone;     // Левый канал
        audio_buffer[i+1] = noise + tone;   // Правый канал
    }

    uint32_t offset = 0;
    while (offset + (chunk_size / 2) <= total_samples) {
        uint32_t released = 0;
        AudioOutBuffer buf;
        memset(&buf, 0, sizeof(AudioOutBuffer));
        buf.buffer = (u32*)&audio_buffer[offset];
        buf.buffer_size = chunk_size;
        buf.data_size = chunk_size;
        
        audoutAppendAudioOutBuffer(&buf);
        AudioOutBuffer* rel_buf = NULL;
        audoutWaitPlayFinish(&rel_buf, &released, 100000000);
        offset += (chunk_size / 2);
    }

    free(audio_buffer);
    audoutStopAudioOut();
    audoutExit();
}

int main(int argc, char** argv) {
    Framebuffer fb;
    framebufferCreate(&fb, nwindowGetDefault(), 1280, 720, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);

    int current_level = 0;
    int player_x = 1, player_y = 1; 
    bool scream_triggered = false;

    while (appletMainLoop()) {
        PadState pad;
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad);
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break;

        u32 stride;
        u32* framebuf = (u32*)framebufferBegin(&fb, &stride);

        if (!scream_triggered) {
            int next_x = player_x;
            int next_y = player_y;
            
            if (kDown & HidNpadButton_Left)  next_x--;
            if (kDown & HidNpadButton_Right) next_x++;
            if (kDown & HidNpadButton_Up)    next_y--;
            if (kDown & HidNpadButton_Down)  next_y++;

            if (levels[current_level][next_y][next_x] != 1) {
                player_x = next_x;
                player_y = next_y;
            }

            if (levels[current_level][player_y][player_x] == 3) {
                current_level++;
                player_x = 1; player_y = 1;
            }

            if (levels[current_level][player_y][player_x] == 4) {
                scream_triggered = true;
            }

            // Рисуем лабиринт
            for (int y = 0; y < 720; y++) {
                for (int x = 0; x < 1280; x++) {
                    int cell_x = x / (1280 / MAZE_W);
                    int cell_y = y / (720 / MAZE_H);
                    u32 color = RGBA8(15, 15, 15, 255);

                    if (cell_x < MAZE_W && cell_y < MAZE_H) {
                        int tile = levels[current_level][cell_y][cell_x];
                        if (tile == 1) color = RGBA8(60, 60, 65, 255);
                        if (tile == 3) color = RGBA8(46, 204, 113, 255);
                    }

                    int block_w = 1280 / MAZE_W;
                    int block_h = 720 / MAZE_H;
                    int px = player_x * block_w + (block_w / 4);
                    int py = player_y * block_h + (block_h / 4);
                    if (x >= px && x < px + 30 && y >= py && y < py + 30) {
                        color = RGBA8(52, 152, 219, 255); // Игрок
                    }

                    framebuf[y * stride / 4 + x] = color;
                }
            }

        } else {
            // ЭКРАН СКРИМЕРА: Генерируем страшные красно-черные мигающие помехи смерти!
            for (int y = 0; y < 720; y++) {
                for (int x = 0; x < 1280; x++) {
                    uint8_t red_noise = rand() % 255;
                    framebuf[y * stride / 4 + x] = RGBA8(red_noise, 0, 0, 255);
                }
            }
            framebufferEnd(&fb);

            // Включаем процедурный ор
            play_procedural_scream();
            
            scream_triggered = false; 
            current_level = 0; 
            player_x = 1; player_y = 1;
            continue;
        }

        framebufferEnd(&fb);
        consoleUpdate(NULL);
    }

    framebufferClose(&fb);
    return 0;
}
