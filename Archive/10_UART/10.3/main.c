#include "uart.h"
#include "servo.h"
#include "keyboard.h"

int main (){
	extern char cOdebranyZnak;
	
	UART_InitWithInt(115200);
	ServoInit(50);
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
		switch(cOdebranyZnak) {
			case '1':
				ServoGoTo(12);
				break;
			case '2':
				ServoGoTo(24);
				break;
			case '3':
				ServoGoTo(36);
				break;
			case 'c':
				ServoCallib();
				break;
		}
	}
}
