#include <LPC21xx.H>
#include "led.h"
#include "keyboard.h"	
#include "timer.h"

int main() {
	LedInit();
	InitTimer0();
	while(1) {
		WaitOnTimer0(100000);
		LedStepLeft();
	}
}
