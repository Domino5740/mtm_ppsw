#include "uart.h"

int main (){
	unsigned int uiMainCtr;
	UART_InitWithInt(115200);
	while(1){
		uiMainCtr++;
	}
}
