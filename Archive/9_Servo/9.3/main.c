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


void Automat() {
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

void ServoInit(unsigned int uiServoFrequency) {
	LedInit();
	sServo.eState = CALLIB;
	Timer0Interrupts_Init((1000000/uiServoFrequency), &Automat);
}
void ServoCallib() {
	sServo.eState = CALLIB;
}
void ServoGoTo(unsigned int uiPosition) {
	sServo.uiDesiredPosition = uiPosition;
}


int main (){
	
	ServoInit(4);
	KeyboardInit();
	
	while(1){
		switch(eKeyboardRead()) {
			case RELEASED:
			break;
			case BUTTON_0:
				ServoCallib();
			break;
			case BUTTON_1:
				ServoGoTo(12);
			break;
			case BUTTON_2:
				ServoGoTo(24);
			break;
			case BUTTON_3:
				ServoGoTo(36);
			break;
		}
	}
}
