#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"
#include "timer.h"

int main() {
	unsigned int uiTest = 1000000;
	LedInit();
	InitTimer0();
	while(1) {
		WaitOnTimer0(uiTest);
		LedStepLeft();
	}
}
