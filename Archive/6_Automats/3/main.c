#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"
/*8108108, na lapku 37499998*/
void Delay(int iLatency) {
	
	int iLoopCounter;
	
	for (iLoopCounter = 0; iLoopCounter < (iLatency*27273); iLoopCounter++) {}
}

int main() {
	enum LedState eLedState = LED_STEP_LEFT;
	unsigned int uiLedPoint;
	LedInit();
	while(1) {
		Delay(250);
		switch(eLedState) {
			case LED_STEP_LEFT:
				uiLedPoint++;
				if((uiLedPoint%4) == 3) {
					eLedState = LED_STEP_RIGHT;
				}
				LedStepLeft();
				break;
			case LED_STEP_RIGHT:
				uiLedPoint--;
				if((uiLedPoint%4) == 0) {
					eLedState = LED_STEP_LEFT;
				}
				LedStepRight();
				break;
		}
	}
}
