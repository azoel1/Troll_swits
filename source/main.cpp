#include <switch.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Основные заголовки libnx для графики и ввода уже включены через #include <switch.h>

#define MAZE_W 15
#define MAZE_H 9

const char* maze_levels[10][MAZE_H] = {
    // ... (оставь свой массив уровней здесь без изменений)
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

int main(int argc, char** argv) {
    // Инициализация графики и консоли
    gfxInitDefault();
    consoleInit(NULL);

    int current_level = 0;
    int player_x = 1, player_y = 1;
    bool scream_triggered = false;

    // Настройка контроллера
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    while(appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break;

        if (!scream_triggered) {
            if (kDown & (HidNpadButton_Up | HidNpadButton_Down | HidNpadButton_Left | HidNpadButton_Right)) {
                int next_x = player_x;
                int next_y = player_y;

                if (kDown & HidNpadButton_Up)    next_y--;
                if (kDown & HidNpadButton_Down)  next_y++;
                if (kDown & HidNpadButton_Left)  next_x--;
                if (kDown & HidNpadButton_Right) next_x++;

                if (next_x >= 0 && next_x < MAZE_W && next_y >= 0 && next_y < MAZE_H) {
                    if (maze_levels[current_level][next_y][next_x] != '#') {
                        player_x = next_x;
                        player_y = next_y;
                    }
                }
            }

            consoleClear();
            for(int i=0; i<MAZE_H; i++) {
                for(int j=0; j<MAZE_W; j++) {
                    if(i == player_y && j == player_x) printf("O");
                    else printf("%c", maze_levels[current_level][i][j]);
                }
                printf("\n");
            }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}
