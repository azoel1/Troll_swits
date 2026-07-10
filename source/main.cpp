#include <switch.h>
#include <stdio.h>

int main(int argc, char** argv) {
    gfxInitDefault();
    consoleInit(NULL);

    printf("\x1b[16;20H RICKROLLED \n"); 
    printf("\x1b[20;15H Never gonna give you up... \n");

    while(appletMainLoop()) {
        hidScanInput();
        if (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_PLUS) break; 
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
