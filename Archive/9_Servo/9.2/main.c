#include "led.h"
#include "timer_interrupts.h"
#include "keyboard.h"
#include <LPC21xx.H>

#define DETECTOR_bm (1<<10)

enum DetectorState {
	ACTIVE, INACTIVE
};

enum ServoState {CALLIB, IDLE, IN_PROGRESS};
struct Servo {
enum ServoState eState;
unsigned int uiCurrentPosition;
unsigned int uiDesiredPosition;
};
struct Servo sServo;

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
	KeyboardInit();
	DetectorInit();
	switch(sServo.eState) {
		case IN_PROGRESS:
			if(sServo.uiCurrentPosition < sServo.uiDesiredPosition) {
				LedStepRight();
				sServo.uiCurrentPosition++;
			}
			else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
				LedStepLeft();
				sServo.uiCurrentPosition--;
			}
			else {
				sServo.eState = IDLE;
			}
		break;
		case IDLE:
			if(sServo.uiCurrentPosition != sServo.uiDesiredPosition) {
				sServo.eState = IN_PROGRESS;
			}
			else {}
		break;
		case CALLIB:
			if(eReadDetector() == ACTIVE) {
				sServo.eState = IDLE;
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
			}
			else {
				LedStepLeft();
			}
		break;
	}
}


int main (){
	
	Timer0Interrupts_Init(200000, &Automaty);
	LedInit();
	
	while(1){
		switch(eKeyboardRead()) {
			case RELEASED:
			break;
			case BUTTON_0:
				sServo.eState = CALLIB;
			break;
			case BUTTON_1:
				sServo.uiDesiredPosition = 12;
			break;
			case BUTTON_2:
				sServo.uiDesiredPosition = 24;
			break;
			case BUTTON_3:
				sServo.uiDesiredPosition = 36;
			break;
		}
	}
}
