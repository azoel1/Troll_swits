#include <switch.h>
#include <stdio.h>
#include <string.h>

// Подключаем наш массив данных встроенного видеоролика skibidi.mp4
extern const uint8_t skibidi_mp4[];
extern const uint32_t skibidi_mp4_size;

int main(int argc, char** argv) {
    // Инициализируем стандартную текстовую консоль поверх экрана
    consoleInit(NULL);

    // Создаем современный объект фреймбуфера (экрана) разрешением 1280x720
    Framebuffer fb;
    framebufferCreate(&fb, nwindowGetDefault(), 1280, 720, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);

    printf("\x1b[2;15H  SKIBIDI PLAYER STARTED \n");
    printf("\x1b[4;12H Press PLUS (+) to safely exit \n");

    uint32_t current_byte_pos = 0;

    // Главный игровой цикл
    while (appletMainLoop()) {
        PadState pad;
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad);
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break; // Выход на Плюс (+)

        // Запрашиваем адрес буфера экрана для рисования пикселей
        u32 stride;
        u32* framebuf = (u32*)framebufferBegin(&fb, &stride);

        if (current_byte_pos + (1280 * 720) < skibidi_mp4_size) {
            for (int y = 0; y < 720; y++) {
                for (int x = 0; x < 1280; x++) {
                    // Извлекаем цвет пикселя из зашитого бинарного массива видео
                    uint8_t byte_val = skibidi_mp4[current_byte_pos + (y * 1280 + x) % 100000];
                    
                    // Формируем RGBA пиксель для современного экрана приставки
                    framebuf[y * stride / 4 + x] = RGBA8(byte_val, byte_val, byte_val, 255);
                }
            }
            // Двигаем плеер вперед по видеопотоку
            current_byte_pos += 4096; 
        } else {
            // Зацикливание видеоролика
            current_byte_pos = 0;
        }

        // Выводим кадр на экран приставки
        framebufferEnd(&fb);
        
        consoleUpdate(NULL);
    }

    // Правильно закрываем графический буфер при выходе
    framebufferClose(&fb);
    consoleExit(NULL);
    return 0;
}
