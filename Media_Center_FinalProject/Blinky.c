#include <LPC17xx.h>
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#define __FI 1    // Font index


// External functions
extern int mp3_player(void);
extern int game(void);
extern int gallery(void);
extern int dodgeBlocksGame(void);

void main_menu(void);

int key_state;

// -----------------------------
//          MAIN MENU
// -----------------------------
void main_menu(void) {
	 int gamestate;
   int gamechoice = 0;
   int gamemenu = 1;

   int isGameMenuDone = 0;
	
	int menu_index = 0;   // Tracks highlighted option

	GLCD_Clear(Black);

	while (1) {
		
		isGameMenuDone = 0;

		// Header section
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(Green);
		GLCD_DisplayString(0, 0, __FI, "     MEDIA HUB MENU     ");

		// Controls info
		GLCD_SetTextColor(Cyan);
		GLCD_DisplayString(8, 0, __FI, " Use UP/DOWN to Scroll  ");
		GLCD_DisplayString(9, 0, __FI, " Press SELECT to Open   ");

		key_state = get_button();

		// Scroll down
		if (key_state == KBD_DOWN) {
			menu_index = (menu_index >= 2) ? 0 : menu_index + 1;
		}
		// Scroll up
		else if (key_state == KBD_UP) {
			menu_index = (menu_index <= 0) ? 2 : menu_index - 1;
		}

		// -----------------------------
		//      MENU DRAWING AREA
		// -----------------------------

		// Gallery Option
		if (menu_index == 0) {
			GLCD_SetBackColor(Blue);  // Highlight color
			GLCD_SetTextColor(White);
			GLCD_DisplayString(3, 0, __FI, "1) Photo Gallery        ");

			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Green);
			GLCD_DisplayString(4, 0, __FI, "2) Music Player         ");
			GLCD_DisplayString(5, 0, __FI, "3) Game Zone            ");

			if (key_state == KBD_SELECT) {
				GLCD_Clear(Blue);
				gallery();
			}
		}

		// Music Option
		else if (menu_index == 1) {
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Green);
			GLCD_DisplayString(3, 0, __FI, "1) Photo Gallery        ");

			GLCD_SetBackColor(Blue); // Highlight music
			GLCD_SetTextColor(White);
			GLCD_DisplayString(4, 0, __FI, "2) Music Player         ");

			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Green);
			GLCD_DisplayString(5, 0, __FI, "3) Game Zone            ");

			if (key_state == KBD_SELECT) {
				GLCD_Clear(Black);
				mp3_player();
				GLCD_Clear(Black);
			}
		}

		// Game Option
		else if (menu_index == 2) {
			GLCD_SetBackColor(Black);
			GLCD_SetTextColor(Green);
			GLCD_DisplayString(3, 0, __FI, "1) Photo Gallery        ");
			GLCD_DisplayString(4, 0, __FI, "2) Music Player         ");

			GLCD_SetBackColor(Blue); // highlight game
			GLCD_SetTextColor(White);
			GLCD_DisplayString(5, 0, __FI, "3) Game Zone            ");

			if (key_state == KBD_SELECT) {
				GLCD_Clear(Black);
				
				
				
			  GLCD_SetBackColor(Black);
        GLCD_SetTextColor(Green);
        GLCD_DisplayString(4, 0, 1, "       Snake Game         ");
        GLCD_DisplayString(5, 0, 1, "       Block Game       ");
        GLCD_DisplayString(7, 0, 1, "       RETURN       ");

    while(!isGameMenuDone){

        while(gamemenu)  {
            gamestate = get_button();

            if (gamestate == KBD_DOWN) {
                if (gamechoice == 2) gamechoice = 0;
                else gamechoice++;
            } else if (gamestate == KBD_UP) {
                if (gamechoice == 0) gamechoice = 2;
                else gamechoice--;
            }

            // Update visual highlight
            if (gamechoice == 0) {    // Snake Game
                GLCD_SetBackColor(Cyan);
                GLCD_SetTextColor(Black);
                GLCD_DisplayString(4, 0, 1, "       Snake Game         ");

                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(5, 0, 1, "       Block Game         ");
                GLCD_DisplayString(7, 0, 1, "       RETURN       ");

                if (gamestate == KBD_SELECT) {
									GLCD_SetTextColor(Green);
									game();
									break;
                }
            }
            else if (gamechoice == 1) { // Block Game
                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(4, 0, 1, "       Snake Game         ");

                GLCD_SetBackColor(Cyan);
                GLCD_SetTextColor(Black);
                GLCD_DisplayString(5, 0, 1, "       Block Game       ");

                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(7, 0, 1, "       RETURN       ");

                if (gamestate == KBD_SELECT) {
									dodgeBlocksGame();
									break;
                }
            }
            else if (gamechoice == 2) { // RETURN
                GLCD_SetBackColor(Black);
                GLCD_SetTextColor(Green);
                GLCD_DisplayString(4, 0, 1, "       Snake Game         ");
                GLCD_DisplayString(5, 0, 1, "       Block Game         ");

                GLCD_SetBackColor(Red);
                GLCD_SetTextColor(White);
                GLCD_DisplayString(7, 0, 1, "       RETURN       ");

                if (gamestate == KBD_SELECT) {
                    GLCD_Clear(Black);
                    isGameMenuDone = 1;
									  break;
                }
            }
        }
		}
	}
}
}
}


// -----------------------------
//            MAIN
// -----------------------------
int main(void) {

	LED_Init();
	GLCD_Init();
	KBD_Init();

	// Intro splash screen
	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);

	GLCD_SetTextColor(Cyan);
	GLCD_DisplayString(2, 0, __FI, "     WELCOME TO THE     ");

	GLCD_SetTextColor(Green);
	GLCD_DisplayString(4, 0, __FI, "       MEDIA HUB        ");

	GLCD_SetTextColor(Yellow);
	GLCD_DisplayString(6, 0, __FI, "      by Hasib Bhuiyan     ");

	GLCD_SetTextColor(Red);
	GLCD_DisplayString(8, 0, __FI, "   Press SELECT to Start");

	// Wait for user
	while (1) {
		if (get_button() == KBD_SELECT) {
			main_menu();
		}
	}
}

