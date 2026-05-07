#include <LPC17xx.h>
#include "GLCD.h"
#include "KBD.h"
#include "LED.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_W   300
#define SCREEN_H   225

// Player size
#define PLAYER_W   40
#define PLAYER_H   12

// Block maximums
#define MAX_BLOCKS 5

int dodgeBlocksGame(void);

// Custom block types / shapes
typedef struct {
    int x, y;
    int w, h;
    int speed;
    int type;
    int colorIndex;
} Block;

// Available block colors (this is for animated cycling)
uint16_t blockColors[] = {
    Blue, Green, Red, Yellow, Magenta, Cyan
};
int colorCount = 6;

// Player position based on Screen 
int playerX = SCREEN_W/2 - PLAYER_W/2;
int playerY = SCREEN_H - 20;

// Game state
Block blocks[MAX_BLOCKS];
int blockScore= 0;
char scoreBuf[20];
int gameOver = 0;

////////////////////////////////////////
// Custom rectangle fill implementation
int row=0;
int col=0;
void DrawRectFilled(int x, int y, int w, int h, uint16_t color) {
    GLCD_SetTextColor(color);
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            GLCD_PutPixel(x + col, y + row);
        }
    }
}

//////////////////////////////
// Spawn a block at the top
void spawnBlock(int index) {
    blocks[index].x = rand() % (SCREEN_W - 40);
    blocks[index].y = -20;

    // Random block type:
    // 0 = small square
    // 1 = wide rectangle
    // 2 = tall rectangle
    // 3 = big square
    blocks[index].type = rand() % 4;

    switch (blocks[index].type) {
        case 0: blocks[index].w = 20; blocks[index].h = 20; break;
        case 1: blocks[index].w = 45; blocks[index].h = 15; break;
        case 2: blocks[index].w = 15; blocks[index].h = 45; break;
        case 3: blocks[index].w = 30; blocks[index].h = 30; break;
    }

    blocks[index].speed = (rand() % 10) + 5;
    blocks[index].colorIndex = rand() % colorCount;
}

//////////////////////////
// Collision detection
int collide(Block *b) {
    if (playerX < b->x + b->w &&
        playerX + PLAYER_W > b->x &&
        playerY < b->y + b->h &&
        playerY + PLAYER_H > b->y)
        return 1;

    return 0;
}

///////////////////////////
// Move & draw all blocks
void updateBlocks(void) {
	  int i=0;
    for (i = 0; i < MAX_BLOCKS; i++) {

        // Erase previous
        DrawRectFilled(blocks[i].x, blocks[i].y, blocks[i].w, blocks[i].h, Black);

        // Move block
        blocks[i].y += blocks[i].speed;

        // If block falls off screen → respawn and increment score
        if (blocks[i].y > SCREEN_H) {
            spawnBlock(i);
            blockScore++;
        }

        // Animate color cycling
        blocks[i].colorIndex = (blocks[i].colorIndex + 1) % colorCount;

        // Draw updated block
        DrawRectFilled(
            blocks[i].x,
            blocks[i].y,
            blocks[i].w,
            blocks[i].h,
            blockColors[blocks[i].colorIndex]
        );

        // Check collision
        if (collide(&blocks[i])) {
            gameOver = 1;
        }
    }
}

//////////////////////////////////////////
// Player movement (Joystick Left/Right)
void updatePlayer(void) {
    int js = get_button();

    // Erase player
    DrawRectFilled(playerX, playerY, PLAYER_W, PLAYER_H, Black);

    if (js == KBD_LEFT) {
        playerX -= 20;
        if (playerX < 0) playerX = 0;
    } else if (js == KBD_RIGHT) {
        playerX += 20;
        if (playerX + PLAYER_W > SCREEN_W) playerX = SCREEN_W - PLAYER_W;
    }

    // Draw updated player (yellow)
    DrawRectFilled(playerX, playerY, PLAYER_W, PLAYER_H, Yellow);
}

////////////////////////////////
// GAME LOOP FUNCTION
volatile int d = 0;
int i=0;
int dodgeBlocksGame(void) {
    GLCD_Clear(Black);
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(Green);

    GLCD_DisplayString(0, 0, 1, "   DODGE THE BLOCKS!   ");
    GLCD_DisplayString(1, 0, 1, "   Use Left/Right Joystick");

    blockScore= 0;
    gameOver = 0;

    // Initialize blocks
    for (i = 0; i < MAX_BLOCKS; i++) {
        spawnBlock(i);
    }

    // Initial draw of player
    DrawRectFilled(playerX, playerY, PLAYER_W, PLAYER_H, Yellow);

    while (!gameOver) {
        updatePlayer();
        updateBlocks();

        // Draw score
        sprintf(scoreBuf, "Score: %d", blockScore);
        GLCD_SetTextColor(Green);
        GLCD_DisplayString(2, 0, 1, (unsigned char*)scoreBuf);

        // Small delay to control speed
        for (d = 0; d < 50000; d++);
    }

    // GAME OVER
    GLCD_Clear(Red);
    GLCD_SetBackColor(Red);
    GLCD_SetTextColor(White);

    sprintf(scoreBuf, " FINAL SCORE: %d ", blockScore);
    GLCD_DisplayString(4, 0, 1, (unsigned char*)scoreBuf);
    GLCD_DisplayString(6, 0, 1, "    GAME OVER!    ");
    GLCD_DisplayString(8, 0, 1, "  PRESS SELECT TO EXIT");

    // Wait for SELECT to exit
    //while (get_button() != KBD_SELECT){};
		GLCD_Clear(Black);
    return 0;
}



