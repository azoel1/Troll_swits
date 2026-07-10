#include <switch.h>
#include <stdio.h>
#include <string.h>

// Подключаем наш массив данных сырых пикселей, созданный из видео
extern const uint8_t raw_video_bin[];
extern const uint32_t raw_video_bin_size;

int main(int argc, char** argv) {
    // Включаем современный графический экран Switch
    Framebuffer fb;
    framebufferCreate(&fb, nwindowGetDefault(), 1280, 720, PIXEL_FORMAT_RGBA_8888, 2);
    framebufferMakeLinear(&fb);

    // Новое компактное разрешение нашего видео кадра
    const int vid_w = 256;
    const int vid_h = 144;
    const uint32_t frame_size = vid_w * vid_h * 3; // 3 байта на пиксель (RGB)

    uint32_t current_offset = 0;

    // Главный цикл приложения
    while (appletMainLoop()) {
        PadState pad;
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad);
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break; // Выход на Плюс (+)

        // Запрашиваем адрес буфера экрана для рисования
        u32 stride;
        u32* framebuf = (u32*)framebufferBegin(&fb, &stride);

        // Если в массиве данных еще есть кадры для показа
        if (current_offset + frame_size <= raw_video_bin_size) {
            
            // Проходим по пикселям экрана Switch (1280x720)
            for (int y = 0; y < 720; y++) {
                for (int x = 0; x < 1280; x++) {
                    // Масштабируем экранные координаты 1280x720 назад в шаг видео 256x144
                    int vx = (x * vid_w) / 1280;
                    int vy = (y * vid_h) / 720;

                    // Находим точное положение пикселя в текущем кадре видеоролика
                    uint32_t pixel_pos = current_offset + (vy * vid_w + vx) * 3;

                    uint8_t r = raw_video_bin[pixel_pos];
                    uint8_t g = raw_video_bin[pixel_pos + 1];
                    uint8_t b = raw_video_bin[pixel_pos + 2];

                    // Записываем цветной пиксель видео прямо в память экрана консоли
                    framebuf[y * stride / 4 + x] = RGBA8(r, g, b, 255);
                }
            }
            
            // Переключаемся на следующий видеокадр
            current_offset += frame_size;
        } else {
            // Если видео доиграло до конца — сбрасываем в ноль и запускаем сначала
            current_offset = 0;
        }

        // Выводим готовый кадр на экран приставки
        framebufferEnd(&fb);
        
        // Задержка под 10 кадров в секунду, чтобы видео шло с правильной скоростью
        svcSleepThread(100000000); 
    }

    // Закрываем графику при выходе
    framebufferClose(&fb);
    return 0;
}

