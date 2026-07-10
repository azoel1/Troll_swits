#include <switch.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // В современных версиях консоль инициализируется одной этой командой
    consoleInit(NULL);

    // Выводим текст на экран
    printf("\x1b[16;20H RICKROLLED \n"); 
    printf("\x1b[20;15H Never gonna give you up... \n");

    while(appletMainLoop()) {
        // Сканируем ввод по современному стандарту (используем PadState)
        PadState pad;
        padConfigureInput(1, HidNpadStyleSet_NpadStandard);
        padInitializeDefault(&pad);
        padUpdate(&pad);

        // Получаем нажатые кнопки
        u64 kDown = padGetButtonsDown(&pad);

        // Выход на кнопку Плюс (+)
        if (kDown & HidNpadButton_Plus) break;

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
