#include <switch.h>
#include <stdio.h>
#include <string.h>

// Подключаем наш массив данных сырого звука, созданный из MP3
extern const uint8_t audio_bin[];
extern const uint32_t audio_bin_size;

int main(int argc, char** argv) {
    // Включаем стандартную текстовую консоль, чтобы видеть надпись
    consoleInit(NULL);

    printf("\x1b[10;15H !!! SKIBIDI AUDIO APP !!! \n");
    printf("\x1b[12;10H Playing built-in music track... \n");
    printf("\x1b[16;12H Press PLUS (+) to safely exit \n");

    // Инициализируем аудиосистему Nintendo Switch
    AudioRendererConfig audio_config = {0};
    audio_config.output_num_channels = 2; // Стерео
    audio_config.output_num_bus = 1;
    audio_config.max_effects = 0;
    audio_config.max_mix_buffers = 8;
    audio_config.max_voices = 1;
    audio_config.max_sub_mixes = 0;
    audio_config.sample_rate = 44100;
    audio_config.rendering_device = AudioRendererRenderingDevice_Dsp;

    // Включаем вывод звука через встроенный динамик/наушники
    audoutInitialize();
    audoutStartAudioOut();

    uint32_t current_audio_offset = 0;
    const uint32_t buffer_size = 4096 * 4; // Размер порции звука для отправки

    // Главный цикл приложения
    while (appletMainLoop()) {
        PadState pad;
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad);
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break; // Выход на Плюс (+)

        // Отправляем порции аудиоданных в звуковой чип приставки
        if (current_audio_offset + buffer_size <= audio_bin_size) {
            uint32_t released_count = 0;
            AudioOutBuffer out_buffer = {0};
            
            out_buffer.buffer_data = (u8*)&audio_bin[current_audio_offset];
            out_buffer.buffer_data_size = buffer_size;
            out_buffer.data_size = buffer_size;

            // Воспроизводим текущий кусок звука
            audoutAppendAudioOutBuffer(&out_buffer);
            
            // Ждем, пока чип проиграет этот кусок перед отправкой следующего
            AudioOutBuffer* released_buffer = NULL;
            audoutWaitPlayFinish(&released_buffer, &released_count, 100000000);

            current_audio_offset += buffer_size;
        } else {
            // Если музыка доиграла до конца — запускаем сначала (бесконечный цикл)
            current_audio_offset = 0;
        }

        consoleUpdate(NULL);
    }

    // Правильно выключаем звук при выходе
    audoutStopAudioOut();
    audoutExit();
    consoleExit(NULL);
    return 0;
}
