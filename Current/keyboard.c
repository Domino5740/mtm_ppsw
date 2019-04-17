#include <LPC21xx.H>
#include "keyboard.h"
#define S0_bm (1<<4)
#define S1_bm (1<<6)
#define S2_bm (1<<5)
#define S3_bm (1<<7)

enum eKeyboardState eKeyboardRead() {
	
	int iButton0Pressed = (IO0PIN & S0_bm);
	int iButton1Pressed = (IO0PIN & S1_bm);
	int iButton2Pressed = (IO0PIN & S2_bm);
	int iButton3Pressed = (IO0PIN & S3_bm);
	
	if(iButton0Pressed == 0) {
		return BUTTON_0;
	}
	else if (iButton1Pressed == 0) {
		return BUTTON_1;
	}
	else if (iButton2Pressed == 0) {
		return BUTTON_2;
	}
	else if (iButton3Pressed == 0) {
		return BUTTON_3;
	}
	return RELEASED;
}

void KeyboardInit() {
	IO0DIR = (IO0DIR & ~(S0_bm | S1_bm | S2_bm | S3_bm));
}
