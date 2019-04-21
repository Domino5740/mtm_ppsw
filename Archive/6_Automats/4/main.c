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
	unsigned int uiLedPoint = 0;
	LedInit();
	
	while(1) {
		Delay(100);
		switch(eLedState) {
			case LED_NO_STEP:
				if(eKeyboardRead() == BUTTON_0) {
					eLedState = LED_STEP_RIGHT;
				}
				break;
			case LED_STEP_RIGHT:
				if((uiLedPoint%4) == 3) {
					eLedState = LED_NO_STEP;
					uiLedPoint = 0;
				}
				else {
					uiLedPoint++;
					LedStepRight();
				}
				break;
		}
	}
}
