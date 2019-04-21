#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"
/*8108108, na lapku 37499998*/
void Delay(int iLatency) {
	
	int iLoopCounter;
	
	for (iLoopCounter = 0; iLoopCounter < (iLatency*27273); iLoopCounter++) {}
}

int main() {
	enum LedState eLedState = STATE0;
	LedInit();
	while(1) {
		Delay(250);
		switch(eLedState) {
			case STATE0:
				eLedState = STATE1;
				LedStepRight();
				break;
			case STATE1:
				eLedState = STATE2;
				LedStepRight();
				break;
			case STATE2:
				eLedState = STATE3;
				LedStepRight();
				break;
			case STATE3:
				eLedState = STATE4;
				LedStepLeft();
				break;
			case STATE4:
				eLedState = STATE5;
				LedStepLeft();
				break;
			case STATE5:
				eLedState = STATE0;
				LedStepLeft();
		}
	}
}
