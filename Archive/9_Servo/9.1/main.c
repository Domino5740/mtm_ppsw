#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
#include <LPC21xx.H>

#define DETECTOR_bm (1<<10)

enum DetectorState {
	ACTIVE, INACTIVE
};

void DetectorInit() {
	IO0DIR = IO0DIR & (~DETECTOR_bm);
}
enum DetectorState eReadDetector() {
	if ((IO0PIN & DETECTOR_bm) == 0) {
		return ACTIVE;
	}
	else {
		return INACTIVE;
	}
}


void Automaty() {
	static enum LedState eLedState = CALLIB;
	KeyboardInit();
	switch(eLedState) {
		case STEP_LEFT:
			if(eKeyboardRead() == BUTTON_1) {
				eLedState = NO_STEP;
			}
			else {
				LedStepLeft();
			}
		break;
		case NO_STEP:
			if(eKeyboardRead() == BUTTON_0) {
				eLedState = STEP_LEFT;
			}
			else if(eKeyboardRead() == BUTTON_2) {
				eLedState = STEP_RIGHT;
			}
			else {}
		break;
		case STEP_RIGHT:
			if(eKeyboardRead() == BUTTON_1) {
				eLedState = NO_STEP;
			}
			else {
				LedStepRight();
			}
		break;
		case CALLIB:
			if(eReadDetector() == ACTIVE) {
				eLedState = NO_STEP;
			}
			else {
				LedStepLeft();
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
