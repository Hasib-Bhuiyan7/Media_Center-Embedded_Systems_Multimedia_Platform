#include <LPC17xx.h>
#include "GLCD.h"
#include "KBD.h"
#include <stdlib.h>
#include <stdio.h>

// --------------------------------------------------------------------
// MANUAL IMPLEMENTATION OF GLCD_FillRect USING GLCD_PutPixel()
// --------------------------------------------------------------------
int iy = 0;
int ix = 0;
void GLCD_FillRect(int x, int y, int w, int h) {
    for (iy = y; iy < y + h; iy++) {
        for (ix = x; ix < x + w; ix++) {
            GLCD_PutPixel(ix, iy);
        }
    }
}

// -----------------------------------------
// DISPLAY CONSTANTS
// -----------------------------------------
#define BG_COLOR      Black
#define FG_COLOR      Green

#define PLAYER_COLOR  Green
#define BLOCK_COLOR   Red

// Player size
#define PLAYER_WIDTH  20
#define PLAYER_HEIGHT 3

// Falling block size
#define BLOCK_SIZE    30

// -----------------------------------------
// GAME VARIABLES
// -----------------------------------------
int playerX = 120;              // Start in the middle (x-direction)
int playerY = 300;              // Bottom of screen

int blockX  = 0;
int blockY  = 0;
int blockSpeed = 2;

int blockScore = 0;
int gameOver = 0;

// -----------------------------------------
// INITIALIZATION
// -----------------------------------------
void initGame() {
    GLCD_Clear(BG_COLOR);
    GLCD_SetTextColor(FG_COLOR);

    playerX = 120;
    playerY = 210;

    blockX = rand() % 240;
    blockY = 0;

    blockSpeed = 2;
    blockScore = 0;
    gameOver = 0;
}

// -----------------------------------------
// DRAW OBJECTS
// -----------------------------------------
void drawPlayer() {
    GLCD_SetTextColor(PLAYER_COLOR);
    GLCD_FillRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT);
}

void clearPlayer() {
    GLCD_SetTextColor(BG_COLOR);
    GLCD_FillRect(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT);
}

void drawBlock() {
    GLCD_SetTextColor(BLOCK_COLOR);
    GLCD_FillRect(blockX, blockY, BLOCK_SIZE, BLOCK_SIZE);
}

void clearBlock() {
    GLCD_SetTextColor(BG_COLOR);
    GLCD_FillRect(blockX, blockY, BLOCK_SIZE, BLOCK_SIZE);
}

void drawScore() {
    char buff[30];
    sprintf(buff, "Score: %d", blockScore);
    GLCD_SetTextColor(FG_COLOR);
    GLCD_DisplayString(0, 0, 1, (unsigned char*)buff);
}

// -----------------------------------------
// COLLISION CHECK
// -----------------------------------------
int checkCollision() {
    if (blockY + BLOCK_SIZE >= playerY) {
        if (blockX < playerX + PLAYER_WIDTH &&
            blockX + BLOCK_SIZE > playerX)
            return 1;
    }
    return 0;
}

// -----------------------------------------
// UPDATE PLAYER (JOYSTICK)
// -----------------------------------------
void updatePlayer() {
    uint32_t key = get_button();

    clearPlayer();

    if (key == KBD_LEFT  && playerX > 0)                      playerX -= 3;
    if (key == KBD_RIGHT && playerX < 240 - PLAYER_WIDTH)     playerX += 3;

    drawPlayer();
}

// -----------------------------------------
// UPDATE BLOCK
// -----------------------------------------
void updateBlock() {
    clearBlock();
    blockY += blockSpeed;

    if (blockY > 230) {
        blockY = 0;
        blockX = rand() % 240;
        blockScore++;

        if (blockScore % 1 == 0)
            blockSpeed++;
    }

    drawBlock();
}

// -----------------------------------------
// MAIN GAME LOOP
// -----------------------------------------
volatile int d = 0;
char buff[20];
void runGame() {
    initGame();
    drawScore();

    while (!gameOver) {
        updatePlayer();
        updateBlock();
        drawScore();

        if (checkCollision())
            gameOver = 1;

        for (d = 0; d < 10000; d++){}; // delay
    }

    GLCD_Clear(BG_COLOR);
    GLCD_SetTextColor(FG_COLOR);
    GLCD_DisplayString(5, 5, 1, (unsigned char*)"GAME OVER!");
    GLCD_DisplayString(6, 5, 1, (unsigned char*)"Final Score:");
    
    sprintf(buff, "%d", blockScore);
    GLCD_DisplayString(7, 5, 1, (unsigned char*)buff);
}

// -----------------------------------------
// MAIN
// -----------------------------------------
int dodgeBlocksGame(void) {
    SystemInit();
    GLCD_Init();
    GLCD_Clear(BG_COLOR);

    GLCD_DisplayString(4, 3, 1, (unsigned char*)"DODGE BLOCKS");
    GLCD_DisplayString(6, 3, 1, (unsigned char*)"Press Select");

    while (get_button() != KBD_SELECT);

    runGame();
	  for (d = 0; d < 10000; d++){}; // delay
	  GLCD_Clear(BG_COLOR);
    return(0);
}

