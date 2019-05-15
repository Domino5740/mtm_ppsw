#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"
/*8108108, na lapku 37499998*/
void Delay(int iLatency) {
	
	int iLoopCounter;
	
	for (iLoopCounter = 0; iLoopCounter < (iLatency*27273); iLoopCounter++) {}
}

int main() {
	
	enum LedState eLedState = LED_NO_STEP;
	unsigned char ucTenCounter = 0;
	
	LedInit();
	
	while(1) {
		Delay(100);
		switch(eLedState) {
			case LED_STEP_LEFT:
				if(eKeyboardRead() == BUTTON_1) {
					eLedState = LED_NO_STEP;
				}
				else {
					LedStepLeft();
				}
				break;
			case LED_NO_STEP:
				if(eKeyboardRead() == BUTTON_0) {
					eLedState = LED_STEP_LEFT;
				}
				else if(eKeyboardRead() == BUTTON_2) {
					eLedState = LED_STEP_RIGHT;
				}
				else if(eKeyboardRead() == BUTTON_3) {
					eLedState = LED_LEFT_RIGHT;
				}
				break;
			case LED_STEP_RIGHT:
				if(eKeyboardRead() == BUTTON_1) {
					eLedState = LED_NO_STEP;
				}
				else {
					LedStepRight();
				}
				break;
			case LED_LEFT_RIGHT:
				if(ucTenCounter%21 == 20) {
					eLedState = LED_NO_STEP;
				}
				else if(ucTenCounter%2 == 0) {
					LedStepLeft();
				}
				else if(ucTenCounter%2 == 1) {
					LedStepRight();
				}
				ucTenCounter++;
		}
	}
}
