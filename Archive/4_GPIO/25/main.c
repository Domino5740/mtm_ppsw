#include <LPC21xx.H>
#define LED0_bm 0x010000
#define LED1_bm 0x020000
#define LED2_bm 0x040000
#define LED3_bm 0x080000
#define LED4_bm 0x100000
#define LED5_bm 0x200000
#define LED6_bm 0x400000
#define LED7_bm 0x800000
#define S0_bm 0x10
#define S1_bm 0x40
#define S2_bm 0x20
#define S3_bm 0x80
/*8108108, na lapku 37499998*/

void Delay(int iLatency) {
	
	int iLoopCounter;
	
	for (iLoopCounter = 0; iLoopCounter < (iLatency*27273); iLoopCounter++) {}
}

void LedInit() {
	IO1DIR = IO1DIR | LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	IO1SET = LED0_bm;
}

void LedOn(unsigned char ucLedIndeks) {
  IO1CLR = (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	switch(ucLedIndeks) {
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
			break;
	}
}

enum eKeyboardState {
	RELEASED,
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3
};

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

enum eDirection {
	LEFT, 
	RIGHT
};

void LedStep(enum eDirection eStepDirection) {
	
	static unsigned int uiLedPoint = 0;

	switch(eStepDirection) {
		case LEFT:
			uiLedPoint++;
			break;
		case RIGHT:
			uiLedPoint--;
			break;
	}
	LedOn(uiLedPoint%4);
}

void LedStepRight() {
	LedStep(RIGHT);
}

void LedStepLeft() {
	LedStep(LEFT);
}

int main() {
	LedInit();
	while(1) {
		switch(eKeyboardRead()) {
			case BUTTON_1:
				LedStepRight();
				break;
			case BUTTON_2:
				LedStepLeft();
				break;
			default:
				break;
		}
	}
}
