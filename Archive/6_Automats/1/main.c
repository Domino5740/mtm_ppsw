#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"
/*8108108, na lapku 37499998*/
void Delay(int iLatency) {
	
	int iLoopCounter;
	
	for (iLoopCounter = 0; iLoopCounter < (iLatency*27273); iLoopCounter++) {}
}

int main() {

	enum LedState eLedState = LED_RIGHT;
	
	LedInit();
	
	while(1) {
		switch(eLedState) {
			case LED_RIGHT:
				eLedState = LED_LEFT;
				LedStepRight();
				break;
			case LED_LEFT:
				eLedState = LED_RIGHT;
				LedStepLeft();
				break;
			default:
				break;
		}
		Delay(250);
	}
}
