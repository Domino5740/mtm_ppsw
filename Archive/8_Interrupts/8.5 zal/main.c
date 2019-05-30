#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"


void Automaty() {
	static enum LedState eLedState = LED_NO_STEP;
	KeyboardInit();
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
			else {}
		break;
		case LED_STEP_RIGHT:
			if(eKeyboardRead() == BUTTON_1) {
				eLedState = LED_NO_STEP;
			}
			else {
				LedStepRight();
			}
		break;
	}
}


int main (){
	
	int iMainCounter;
	Timer0Interrupts_Init(20000, &Automaty);
	LedInit();
	
	while(1){
		iMainCounter++;
	}
}
