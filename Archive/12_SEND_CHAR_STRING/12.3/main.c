#include "uart.h"
#include "command_decoder.h"
#include "led.h"
#include <LPC21xx.H>

extern struct ReceiverBuffer sReceiverBuffer;
extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern struct TransmitterBuffer sTransmitterBuffer;

int main(){
	
	UART_InitWithInt(9600);
	
	while(1){
		if(sTransmitterBuffer.eStatus == FREE) {
			Transmitter_SendString("test123");
		}
	}
}
