#include "led.h"
#include "keyboard.h"

void Delay(int iLatency) {
	
	int iLoopCounter;
	
	for (iLoopCounter = 0; iLoopCounter < (iLatency*27273); iLoopCounter++) {}
}

int main() {
	LedInit();
	Delay(500);
	while(1) {
		Delay(100);
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
