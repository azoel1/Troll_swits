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

    // Разрешение нашего сырого видео кадра
    const int vid_w = 400;
    const int vid_h = 240;
    const uint32_t frame_size = vid_w * vid_h * 3; // 3 байта на пиксель (RGB)

    uint32_t current_offset = 0;

    // Главный цикл
    while (appletMainLoop()) {
        PadState pad;
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad);
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break; // Выход на Плюс (+)

        // Берем буфер экрана
        u32 stride;
        u32* framebuf = (u32*)framebufferBegin(&fb, &stride);

        // Если в массиве еще есть кадры
        if (current_offset + frame_size < raw_video_bin_size) {
            
            // Проходим по пикселям экрана Switch (1280x720)
            for (int y = 0; y < 720; y++) {
                for (int x = 0; x < 1280; x++) {
                    // Масштабируем координаты экрана 1280x720 назад в шаг видео 400x240
                    int vx = (x * vid_w) / 1280;
                    int vy = (y * vid_h) / 720;

                    // Находим положение этого пикселя в текущем кадре видео
                    uint32_t pixel_pos = current_offset + (vy * vid_w + vx) * 3;

                    uint8_t r = raw_video_bin[pixel_pos];
                    uint8_t g = raw_video_bin[pixel_pos + 1];
                    uint8_t b = raw_video_bin[pixel_pos + 2];

                    // Рисуем цветной пиксель видео прямо на экран консоли!
                    framebuf[y * stride / 4 + x] = RGBA8(r, g, b, 255);
                }
            }
            
            // Переключаемся на следующий кадр видеоролика
            current_offset += frame_size;
        } else {
            // Если видео кончилось — запускаем сначала (эффект гифки)
            current_offset = 0;
        }

        framebufferEnd(&fb);
        
        // Небольшая задержка, чтобы видео не крутилось слишком бешено (примерно 30 кадров в секунду)
        svcSleepThread(33000000); 
    }

    framebufferClose(&fb);
    return 0;
}
