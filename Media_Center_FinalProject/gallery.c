#include <LPC17xx.H>
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#define __FI 1    // Font

// Gallery Images (DO NOT MODIFY)
#include "basketball.c"
#include "soccerball.c"
#include "car.c"
#include "bird.c"


// ------------------------------------------
//                GALLERY MODE
// ------------------------------------------
int gallery(void) {

	int current = 0;
	int key = 0;

	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);

	while (1) {

		// Header
		GLCD_SetTextColor(Green);
		GLCD_DisplayString(0, 0, __FI, "        GALLERY        ");

		// Instructions
		GLCD_SetTextColor(Cyan);
		GLCD_DisplayString(8, 0, __FI, " LEFT / RIGHT  to Scroll ");
		GLCD_DisplayString(9, 0, __FI, " SELECT       to Return  ");

		key = get_button();

		// Scroll Left
		if (key == KBD_LEFT) {
			current = (current == 0) ? 3 : current - 1;
			GLCD_Clear(Black);
		}

		// Scroll Right
		else if (key == KBD_RIGHT) {
			current = (current == 3) ? 0 : current + 1;
			GLCD_Clear(Black);
		}

		// Return to Main Menu
		else if (key == KBD_SELECT) {
			GLCD_Clear(Black);
			return 0;
		}

		// ------------------------------------------
		//          DISPLAY CURRENT IMAGE
		// ------------------------------------------
		if(current == 0)  {
						GLCD_Bitmap (80, 55, 155,  134, (unsigned char *)BASKETBALL_IMAGE_pixel_data);	
			}
			else if (current == 1) {
						GLCD_Bitmap (80, 55, 150,  134, (unsigned char *)SOCCER_IMAGE_pixel_data);
			}
			else if (current == 2) {
						GLCD_Bitmap (80, 55, 150,  84, (unsigned char *)CAR_IMAGE_pixel_data);	
			}
			else if (current == 3)
			{
						GLCD_Bitmap (80, 25, 150,  84, (unsigned char *)BIRD_IMAGE_pixel_data);
			}
		}
}
