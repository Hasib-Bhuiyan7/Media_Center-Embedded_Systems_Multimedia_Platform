#include "stdio.h"
#include "stdlib.h"
#include "LPC17xx.h"
#include "KBD.h"
#include "GLCD.h"
#include "LED.h"


#define UP      0
#define RIGHT   1
#define DOWN    2
#define LEFT    3

// ---------------------------
// Data model (renamed fields)
// ---------------------------
// The snake is stored as an array of (row, col) pairs:
//   body[i][0] = X (row, vertical; increases downwards on the GLCD)
//   body[i][1] = Y (col, horizontal; increases to the right)
int body[100][2];
int head_x;                 // pending/new head row
int head_y;                 // pending/new head col
int snake_len;              // number of segments
int direction;              // current direction enum
int prev_direction;         // previous direction (kept for compatibility)
int speed;                  // delay factor (same semantic as before)

// Food coordinates (row, col)
int food_x, food_y;

// Joystick / button state
int js_current = KBD_UP;
int js_previous = KBD_UP;

// Score display buffer
char score_str[20];

// Flags
int isHit = 0;
int score = 0;
int isGameDone = 0;

// Prototypes (names preserved for external linkage)
int game(void);
int restart(void);
void eat_food(void);
void extend_body(void);
void state_check(void);
void update(void);
void dir(int);

// ---------------------------
// Delay (busy loop) - unchanged
// ---------------------------
void delay (int count){
    int val = 10000000;
    val /= count;
    while(val--);
}

// ---------------------------
// Place food at a random cell
// ---------------------------
void eat_food(){

    // Row 0 is used for the score display, so place food from row 1..8
    food_x = (rand() % 8) + 1;  // rows 1..8
    food_y = rand() % 19;       // cols 0..18

    // If food spawns on the head, try again (note: body-check left as original)
    if (food_x == body[0][0])
        if (food_y == body[0][1])
            eat_food();

    // Draw food (use red as before)
    GLCD_SetTextColor(Red);
    GLCD_DisplayChar(food_x, food_y, 1, 0x81);
}

// ---------------------------
// Grow and increment score
// ---------------------------
void extend_body(){
    snake_len++;
    score += 1;
}

// ---------------------------
// Check for collisions & eating
// ---------------------------
void state_check(){
    int i;

    // If head is on food => eat and grow
    if (food_x == body[0][0])
        if (food_y == body[0][1]) {
            extend_body();
            eat_food();
        }

    // Self-collision: head collides with any body segment
    for (i = 1; i < snake_len; i++) {
        if (body[0][0] == body[i][0])
            if (body[0][1] == body[i][1])
                isHit = 1;
    }
}
// Update display & internal body positions
void update(){
    int i;

    GLCD_SetTextColor(Green);

    // 1) shift body segments so each takes the position of the previous one
    // 2) set head (body[0]) to the pending (head_x, head_y)
    // 3) draw head glyph depending on direction and draw body glyph for segments
    //
    // Note: X = row (vertical), Y = col (horizontal).  Row 0 is UI row.
    if (direction == UP){
        for (i = snake_len; i > 0; i--) {
            if (i - 1 == 0) {
                // Apply new head coordinates
                body[0][1] = head_y;
                body[0][0] = head_x;
            } else {
                // Erase the old tail cell and shift
                GLCD_DisplayChar(body[i-1][0], body[i-1][1], 1, ' ');
                body[i-1][1] = body[i-2][1];
                body[i-1][0] = body[i-2][0];
            }
        }
        // Draw head and body: head uses a direction-specific glyph, body uses a new glyph
        for (i = 1; i < snake_len; i++) {
					  GLCD_SetTextColor(Green);
            GLCD_DisplayChar(body[0][0], body[0][1], 1, 0x81); //0x95);  // new head-up glyph
            GLCD_DisplayChar(body[i][0], body[i][1], 1, 0x81); //0xA3);  // new body glyph
        }
        delay(speed);
    }
    else if (direction == RIGHT){
        for (i = snake_len; i > 0; i--) {
            if (i - 1 == 0) {
                body[0][1] = head_y;
                body[0][0] = head_x;
            } else {
                GLCD_DisplayChar(body[i-1][0], body[i-1][1], 1, ' ');
                body[i-1][1] = body[i-2][1];
                body[i-1][0] = body[i-2][0];
            }
        }
        for (i = 1; i < snake_len; i++) {
					  GLCD_SetTextColor(Green);
            GLCD_DisplayChar(body[0][0], body[0][1], 1, 0x81); //0x9B);  // new head-right glyph
            GLCD_DisplayChar(body[i][0], body[i][1], 1, 0x81); //0xA3);  // new body glyph
        }
        delay(speed);
    }
    else if (direction == DOWN){
        for (i = snake_len; i > 0; i--) {
            if (i - 1 == 0) {
                body[0][1] = head_y;
                body[0][0] = head_x;
            } else {
                GLCD_DisplayChar(body[i-1][0], body[i-1][1], 1, ' ');
                body[i-1][1] = body[i-2][1];
                body[i-1][0] = body[i-2][0];
            }
        }
        for (i = 1; i < snake_len; i++) {
					  GLCD_SetTextColor(Green);
            GLCD_DisplayChar(body[0][0], body[0][1], 1, 0x81); //0x97);  // new head-down glyph
            GLCD_DisplayChar(body[i][0], body[i][1], 1, 0x81); //0xA3);  // new body glyph
        }
        delay(speed);
    }
    else if (direction == LEFT){
        for (i = snake_len; i > 0; i--) {
            if (i - 1 == 0) {
                body[0][1] = head_y;
                body[0][0] = head_x;
            } else {
                GLCD_DisplayChar(body[i-1][0], body[i-1][1], 1, ' ');
                body[i-1][1] = body[i-2][1];
                body[i-1][0] = body[i-2][0];
            }
        }
        for (i = 1; i < snake_len; i++) {
					  GLCD_SetTextColor(Green);
            GLCD_DisplayChar(body[0][0], body[0][1], 1, 0x81); //0x99);  // new head-left glyph
            GLCD_DisplayChar(body[i][0], body[i][1], 1, 0x81); //0xA3);  // new body glyph
        }
        delay(speed);
    }

    // After moving and drawing, re-check state (food or self-hit)
    state_check();
}

// ---------------------------
// Movement control & wrap-around
// ---------------------------
// This retains your original logic: joystick inputs immediately cause a
// step in the requested orthogonal direction (only when allowed).
void dir(int joystick_val){

    switch (joystick_val) {
        case KBD_UP:
            // Only accept a vertical change if previous joystick was horizontal
            if (js_previous == KBD_LEFT || js_previous == KBD_RIGHT) {
                head_x--;
                direction = UP;
                prev_direction = direction;
                js_previous = joystick_val;
                update();
            }
            break;

        case KBD_RIGHT:
            if (js_previous == KBD_UP || js_previous == KBD_DOWN) {
                head_y++;
                direction = RIGHT;
                prev_direction = direction;
                js_previous = joystick_val;
                update();
            }
            break;

        case KBD_DOWN:
            if (js_previous == KBD_LEFT || js_previous == KBD_RIGHT) {
                head_x++;
                direction = DOWN;
                prev_direction = direction;
                js_previous = joystick_val;
                update();
            }
            break;

        case KBD_LEFT:
            if (js_previous == KBD_UP || js_previous == KBD_DOWN) {
                head_y--;
                direction = LEFT;
                prev_direction = direction;
                js_previous = joystick_val;
                update();
            }
            break;

        default:
            // No explicit joystick change — continue moving in current direction; This is default movement
            switch (direction) {
                case RIGHT:
                    head_y++;
                    if (head_y > 20){
											isHit = 1;
										}
                    update();
                    break;
										
                case LEFT:
                    head_y--;
                    if (head_y < 0){
											isHit = 1;
										}
                    update();
                    break;
										
                case DOWN:
                    head_x++;
                    if (head_x > 9){
											isHit = 1;
										}
                    update();
                    break;
										
                case UP:
                    head_x--;
                    if (head_x < 0){
											isHit = 1;
										}
                    update();
                    break;
            }
						state_check();
            break;
    }
}

// ---------------------------
// Restart wrapper (kept simple)
// ---------------------------
int restart(void)
{
    game();
    return 0;
}

// ---------------------------
// Main game: menu, initialization, loop
// ---------------------------
int game(){
    int js_menu;
    int active_sel = 0;
    int sel_menu = 1;

    isGameDone = 0;

    // Initialize game state (same defaults as before)
    score = 0;
    isHit = 0;
    direction = UP;
    prev_direction = UP;
    js_current = KBD_UP;
    js_previous = KBD_UP;
    snake_len = 2;
    head_x = 4;
    head_y = 9;

    KBD_Init();
    GLCD_Init();

    //black background + green text theme
    GLCD_Clear(Black);
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(Green);
    GLCD_DisplayString(0, 0, 1, "     SNAKE GAME     ");

    // Menu area
    GLCD_SetBackColor(Black);
    GLCD_SetTextColor(Green);
    GLCD_DisplayString(4, 0, 1, "       EASY         ");
    GLCD_DisplayString(5, 0, 1, "       HARD       ");
    GLCD_DisplayString(7, 0, 1, "       RETURN       ");

    while(!isGameDone){

        while(sel_menu)  {
            js_menu = get_button();

            if (js_menu == KBD_DOWN) {
                if (active_sel == 2) active_sel = 0;
                else active_sel++;
            } else if (js_menu == KBD_UP) {
                if (active_sel == 0) active_sel = 2;
                else active_sel--;
            }

            // Update visual highlight
            if (active_sel == 0) {    // EASY
                GLCD_SetBackColor(Cyan);
                GLCD_SetTextColor(Black);
                GLCD_DisplayString(4, 0, 1, "       EASY         ");

                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(5, 0, 1, "       HARD         ");
                GLCD_DisplayString(7, 0, 1, "       RETURN       ");

                if (js_menu == KBD_SELECT) {
                    speed = 1;
                    break;
                }
            }
            else if (active_sel == 1) { // HARD
                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(4, 0, 1, "       EASY         ");

                GLCD_SetBackColor(Cyan);
                GLCD_SetTextColor(Black);
                GLCD_DisplayString(5, 0, 1, "       HARD       ");

                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(7, 0, 1, "       RETURN       ");

                if (js_menu == KBD_SELECT) {
                    speed = 3;
                    break;
                }
            }
            else if (active_sel == 2) { // RETURN
                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(4, 0, 1, "       EASY         ");
                GLCD_DisplayString(5, 0, 1, "       HARD         ");

                GLCD_SetBackColor(Red);
                GLCD_SetTextColor(White);
                GLCD_DisplayString(7, 0, 1, "       RETURN       ");

                if (js_menu == KBD_SELECT) {
                    GLCD_Clear(Black);
                    isGameDone = 1;
                    return 0;
                }
            }
        }

        // Start game; black theme and place initial food
        GLCD_Clear(Black);
        GLCD_SetBackColor(Black);
        eat_food();
				update();

        // Main play loop
        while (isHit == 0) {
            js_current = get_button();
            dir(js_current);

            // Display score at row 0, column 5
            sprintf(score_str, "SCORE: %d", score);
            GLCD_SetTextColor(Green);
            GLCD_DisplayString(0, 5, 1, (unsigned char *)score_str);
        }

        // Game over handling 
        if (isHit == 1) {
            GLCD_Clear(Maroon);
            GLCD_SetBackColor(Maroon);
            GLCD_SetTextColor(White);
            sprintf(score_str, "  FINAL SCORE: %d  ", score);
            GLCD_DisplayString(3, 0, 1, (unsigned char *)score_str);
            GLCD_DisplayString(5, 0, 1, "    YOU LOST LMAO   ");
            delay(1); delay(1); delay(1); delay(1);

            restart();
        }

        delay(1);
    }
    return 0;
}


