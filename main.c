#include "toolbox.h"
#include "input.h"
#include "stdbool.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "gameOver.h"
#include "alphabet.h"

struct Coord {
    int x, y;
};

COLOR snakeColor = 0b000101111100000;
        //RGB15(0, 31, 2);
const COLOR cherryRed = 0b000100101011111;
        //RGB15(31, 10, 2);



volatile unsigned short* bg0_control = (volatile unsigned short*) 0x4000008;


/* return a pointer to one of the 4 character blocks (0-3) */
volatile unsigned short* char_block(unsigned long block) {
    /* they are each 16K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x4000));
}

/* return a pointer to one of the 32 screen blocks (0-31) */
volatile unsigned short* screen_block(unsigned long block) {
    /* they are each 2K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x800));
}

#define menuScreenBlock     ((u16*)screen_block(16))
#define gameOverScreenBlock ((u16*)screen_block(20))
#define tilesetBlock        ((u32*)char_block(0))

void drawWord(int x, int y, char* text) {
    for(int i = 0; i < strlen(text); i++) {
        if(text[i] != ' ') menuScreenBlock[y * 32 + x] = text[i] - 0x40;
        x++;
    }
}

void drawWordAndPalette(int x, int y, char* text, int palette) {
    for(int i = 0; i < strlen(text); i++) {
        if(text[i] != ' ') menuScreenBlock[y * 32 + x] = text[i] - 0x40 | (palette << 12);
        x++;
    }
}

void setPalette(int x, int y, int palette) {
    menuScreenBlock[y * 32 + x] = menuScreenBlock[y * 32 + x] | (palette << 12);
}

volatile unsigned short* bg_palette = (volatile unsigned short*) 0x5000000;
u16 palettes[50];

int speed = 0;
int berries = 1;
int score = 0;
bool sophieOn = false;

void toggleSophie() {
    if(!sophieOn) {
        snakeColor = RGB15(31, 10, 15);
        drawWordAndPalette(19, 18, "SOPHIE MODE", 3);
        sophieOn = true;
    } else {
        snakeColor = RGB15(0, 31, 2);
        drawWordAndPalette(19, 18, "SOPHIE MODE", 4);
        sophieOn = false;
    }
}

void menu() {
    REG_DISPCNT = DCNT_MODE0 | DCNT_BG0;
    palettes[1] = CLR_WHITE;
    palettes[17] = RGB15(5, 15, 31);
    palettes[33] = RGB15(0, 0, 31);
    palettes[49] = RGB15(31, 10, 15);
    for(int i = 0; i < 50; i++) {
        bg_palette[i] = palettes[i];
    }

    for(int i = 0; i < 27 * 8; i++) {
        tilesetBlock[i] = alphabet_image[i/8][i%8];
    }



    //SNAKE FOR GBA

    drawWord(12, 2, "SNAKE");
    drawWord(11, 3, "FOR GBA");

    drawWord(12, 6, "SPEED");

    drawWord(7, 8, "SLOW");
    drawWord(18, 8, "FAST");

    drawWord(11, 12, "BERRIES");

    drawWord(6, 14, "ONE");
    drawWord(12, 14, "THREE");
    drawWord(20, 14, "FIVE");

    //SOPHIE MODE
    drawWordAndPalette(19, 18, "SOPHIE MODE", 4);


    /* set all control the bits in this register */
    *bg0_control = 0 |    /* priority, 0 is highest, 3 is lowest */
                   (0 << 2)  |       /* the char block the image data is stored in */
                   (0 << 6)  |       /* the mosaic flag */
                   (0 << 7)  |       /* color mode, 0 is 16 colors, 1 is 256 colors */
                   (16 << 8) |       /* the screen block the tile data is stored in */
                   (1 << 13) |       /* wrapping flag */
                   (0 << 14);        /* bg size, 0 is 256x256 */
    bool gameStart = false;
    bool speedSelect = false;
    bool tryAgain = false;

    for(int x = 7; x < 11; x++) {
        setPalette(x, 8, 1);
    }

    speed = 0;
    berries = 3;
    sophieOn = false;
    snakeColor = RGB15(0, 31, 2);

    while(!gameStart){
        while(!speedSelect) {
            key_poll();
            if(speed == 0) {
                if(tryAgain) {
                    drawWordAndPalette(7, 8, "SLOW", 1);
                    tryAgain = false;
                }
                if(key_hit(KEY_RIGHT)) {
                    drawWordAndPalette(7, 8, "SLOW", 0);
                    speed = 1;
                    drawWordAndPalette(18, 8, "FAST", 1);
                } else if(key_hit(KEY_A) || key_hit(KEY_START)) {
                    drawWordAndPalette(7, 8, "SLOW", 2);
                    drawWordAndPalette(12, 14, "THREE", 1);
                    berries = 3;
                    speedSelect = true;
                }
            } else if(speed == 1) {
                if(tryAgain) {
                    drawWordAndPalette(18, 8, "FAST", 1);
                    tryAgain = false;
                }
                if(key_hit(KEY_LEFT)) {
                    drawWordAndPalette(7, 8, "SLOW", 1);
                    speed = 0;
                    drawWordAndPalette(18, 8, "FAST", 0);
                } else if(key_hit(KEY_A) || key_hit(KEY_START)) {
                    drawWordAndPalette(18, 8, "FAST", 2);
                    drawWordAndPalette(12, 14, "THREE", 1);
                    berries = 3;
                    speedSelect = true;
                }
            }
        }
        tryAgain = true;
        key_poll();
        if(key_is_down(KEY_L) && key_hit(KEY_R) || key_is_down(KEY_R) && key_hit(KEY_L)) {
            toggleSophie();
        }
        if(berries == 1) {
            if(key_hit(KEY_RIGHT)) {
                drawWordAndPalette(6, 14, "ONE", 0);
                drawWordAndPalette(12, 14, "THREE", 1);
                berries = 3;
            } else if(key_hit(KEY_B)) {
                drawWordAndPalette(6, 14, "ONE", 0);
                speedSelect = false;
            } else if(key_hit(KEY_A)) {
                gameStart = true;
            }
        } else if(berries == 3) {
            if(key_hit(KEY_LEFT)) {
                drawWordAndPalette(6, 14, "ONE", 1);
                drawWordAndPalette(12, 14, "THREE", 0);
                berries = 1;
            } else if(key_hit(KEY_RIGHT)) {
                drawWordAndPalette(20, 14, "FIVE", 1);
                drawWordAndPalette(12, 14, "THREE", 0);
                berries = 5;
            } else if(key_hit(KEY_B)) {
                drawWordAndPalette(12, 14, "THREE", 0);
                speedSelect = false;
            } else if(key_hit(KEY_A)) {
                gameStart = true;
            }
        } else if(berries == 5) {
            if(key_hit(KEY_LEFT)) {
                drawWordAndPalette(20, 14, "FIVE", 0);
                drawWordAndPalette(12, 14, "THREE", 1);
                berries = 3;
            } else if(key_hit(KEY_B)) {
                drawWordAndPalette(20, 14, "FIVE", 0);
                speedSelect = false;
            } else if(key_hit(KEY_A)) {
                gameStart = true;
            }
        }

    }
}

void spawnBerry(int seed) {
    int berryX, berryY;
    do {
        srand(seed);
        berryX = rand() % SCREEN_WIDTH;
        berryY = rand() % SCREEN_HEIGHT;
        seed-=5;
    } while(*(vid_mem + berryX + (berryY * SCREEN_WIDTH)) == snakeColor || *(vid_mem + berryX + (berryY * SCREEN_WIDTH)) == cherryRed);
    m3_plot(berryX, berryY, cherryRed);
    return;
}

int main() {

    menu();

    int buffer;
    struct Coord snakeArr[SCREEN_WIDTH * SCREEN_HEIGHT];
    int currentDir, prevDir, xHead, yHead, bCounter, snakeLength, seed;
    bool gameOver, gamePause, restartGame;


    while(1) {

        REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
            vid_mem[i] = 0;
        }

        buffer = speed==0 ? 7000 : 3000;
        currentDir = 3;
        prevDir = 3;
        seed = 0;
        xHead = SCREEN_WIDTH / 2;
        yHead = SCREEN_HEIGHT / 2;
        gameOver = false;
        gamePause = false;

        bCounter = 0;
        snakeLength = 5;
        for (int i = 0; i < snakeLength; i++) {
            snakeArr[i].x = xHead - i;
            snakeArr[i].y = yHead;
            m3_plot(xHead - i, yHead, snakeColor);
        }

        for(int i = 0; i < berries; i++) spawnBerry(time(NULL));


        while (!gameOver) {
            key_poll();

            if (key_hit(KEY_UP)) {
                if (currentDir != 1 && prevDir != 1) currentDir = 0;
                seed+=5;
            }
            if (key_hit(KEY_DOWN)) {
                if (currentDir != 0 && prevDir != 0) currentDir = 1;
                seed-=4;
            }
            if (key_hit(KEY_LEFT)) {
                if (currentDir != 3 && prevDir != 3) currentDir = 2;
                seed-=2;
            }
            if (key_hit(KEY_RIGHT)) {
                if (currentDir != 2 && prevDir != 2) currentDir = 3;
                seed++;
            }
            if (key_hit(KEY_START)) {
                gamePause = !gamePause;
                seed*=2;
            }
            if (key_hit(KEY_L)) {
                buffer += 25;
            }
            if (key_hit(KEY_R)) {
                buffer -= 25;
            }

            if (!gamePause) bCounter++;
            else bCounter = 0;

            if (bCounter == buffer) {
                if (currentDir == 0) {
                    yHead--;
                } else if (currentDir == 1) {
                    yHead++;
                } else if (currentDir == 2) {
                    xHead--;
                } else {
                    xHead++;
                }
                prevDir = currentDir;

                if (*(vid_mem + xHead + (yHead * SCREEN_WIDTH)) == cherryRed) {
                    snakeArr[snakeLength].x = snakeArr[snakeLength - 1].x;
                    snakeArr[snakeLength].y = snakeArr[snakeLength - 1].y;
                    snakeLength++;
                    score++; 
                    spawnBerry(seed);
                } else {
                    m3_plot(snakeArr[snakeLength - 1].x, snakeArr[snakeLength - 1].y, CLR_BLACK);
                }


                for (int i = snakeLength - 1; i > 0; i--) {
                    snakeArr[i] = snakeArr[i - 1];
                }

                if (*(vid_mem + xHead + (yHead * SCREEN_WIDTH)) == snakeColor) gameOver = true;
                else if (xHead >= SCREEN_WIDTH || xHead < 0) gameOver = true;
                else if (yHead >= SCREEN_HEIGHT || yHead < 0) gameOver = true;


                snakeArr[0].x = xHead;
                snakeArr[0].y = yHead;

                m3_plot(xHead, yHead, snakeColor);
                bCounter = 0;
            }
        };

        int itrIdx = 0;
        for (itrIdx = 0; itrIdx < 21713; itrIdx++) {
            vid_mem[itrIdx] = gameOverImg[itrIdx];
        }
        while (itrIdx < SCREEN_HEIGHT * SCREEN_WIDTH) {
            vid_mem[itrIdx] = 0;
            itrIdx++;
        }

        restartGame = false;
        while(!restartGame){
            key_poll();
            if(key_hit(KEY_START)) restartGame = true;
        }
        menu();
    }

    return 0;
}