#include <switch.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAZE_W 15
#define MAZE_H 9

const char* maze_levels[10][MAZE_H] = {
    { "###############", "#S...#......F.#", "###.#.###.#.###", "#.....#.#.#...#", "#.#####.#####.#", "#...#.......#.#", "###.#.###.#.###", "#.....#.#.#...#", "###############" },
    { "###############", "#S#...#...#...#", "#.#.#.#.#.#.#.#", "#...#...#...#.#", "#######.#####.#", "#...........#.#", "#.#########.#.#", "#...........F#", "###############" },
    { "###############", "#S....#.......#", "#####.#.#######", "#...#.#.#.....#", "#.#####.#####.#", "#.#####.###.#.#", "#.#####.....#.#", "#F###########.#", "###############" },
    { "###############", "#S#.....#.....#", "#.#.###.#.#####", "#.#.#.#.#.#...#", "#...#.#...#.#.#", "#####.#####.#.#", "#.....#.....#.#", "#F###########.#", "###############" },
    { "###############", "#S..........F.#", "#.###########.#", "#.............#", "###########.###", "#.............#", "#.###########.#", "#.............#", "###############" },
    { "###############", "#S#...#...#...#", "#.#.#.#.#.#.#.#", "#...#...#...#.#", "#######.#####.#", "#...........#.#", "#.#########.#.#", "#...........F#", "###############" },
    { "###############", "#S....#.......#", "#####.#.#######", "#...#.#.#.....#", "#.#####.#####.#", "#.#####.#...#.#", "#.#####...#.#.#", "#F#########...#", "###############" },
    { "###############", "#S#.....#.....#", "#.#.###.#.#####", "#.#.#.#.#.#...#", "#...#.#...#.#.#", "#####.#####.#.#", "#.....#.....#.#", "#F###########.#", "###############" },
    { "###############", "#S...#......F.#", "###.#.###.#.###", "#.....#.#.#...#", "#.#####.#####.#", "#...#.......#.#", "###.#.###.#.###", "#.....#.#.#...#", "###############" },
    { "###############", "#S............#", "#######.#####.#", "#.....#.#...#.#", "#.###.#.#.#.#.#", "#......X..#...#", "#.###########.#", "#...........F.#", "###############" }
};

void play_scream() {
    audoutInitialize();
    audoutStartAudioOut();
    uint32_t total_samples = 44100 * 2 * 2;
    int16_t* buf = (int16_t*)malloc(total_samples * sizeof(int16_t));
    for(uint32_t i=0; i<total_samples; i++) buf[i] = (rand() % 32767) - 16384;
    AudioOutBuffer audio_buf;
    memset(&audio_buf, 0, sizeof(AudioOutBuffer));
    audio_buf.buffer = (u32*)buf;
    audio_buf.buffer_size = total_samples * 2;
    audio_buf.data_size = total_samples * 2;
    audoutAppendAudioOutBuffer(&audio_buf);
    uint32_t released = 0;
    AudioOutBuffer* out_buf = NULL;
    audoutWaitPlayFinish(&out_buf, &released, 1000000000);
    audoutStopAudioOut();
    audoutExit();
    free(buf);
}

int main(int argc, char** argv) {
    gfxInitDefault();
    consoleInit(NULL);
    
    int current_level = 0;
    int player_x = 1, player_y = 1;
    bool scream_triggered = false;
    u64 last_buttons = 0; // Для отслеживания состояния кнопок

    while(appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) break;

        if (!scream_triggered) {
            // ДВИЖЕНИЕ ТОЛЬКО ПРИ НОВОМ НАЖАТИИ
            if (kDown & (KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT)) {
                int next_x = player_x;
                int next_y = player_y;
                if (kDown & KEY_UP) next_y--;
                else if (kDown & KEY_DOWN) next_y++;
                else if (kDown & KEY_LEFT) next_x--;
                else if (kDown & KEY_RIGHT) next_x++;

                if (next_x >= 0 && next_x < MAZE_W && next_y >= 0 && next_y < MAZE_H) {
                    if (maze_levels[current_level][next_y][next_x] != '#') {
                        player_x = next_x;
                        player_y = next_y;
                    }
                }

                // Проверки
                if (maze_levels[current_level][player_y][player_x] == 'F') {
                    current_level = (current_level + 1) % 10;
                    player_x = 1; player_y = 1;
                }
                if (maze_levels[current_level][player_y][player_x] == 'X') {
                    scream_triggered = true;
                }
            }

            // Рендеринг текста (вместо графики для простоты отладки движения)
            consoleClear();
            for(int i=0; i<MAZE_H; i++) {
                for(int j=0; j<MAZE_W; j++) {
                    if(i == player_y && j == player_x) printf("O");
                    else printf("%c", maze_levels[current_level][i][j]);
                }
                printf("\n");
            }
        } else {
            printf("\n\n\n     RICKROLLED!!!     \n");
            play_scream();
            scream_triggered = false;
            player_x = 1; player_y = 1;
        }
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }
    gfxExit();
    return 0;
}
