#include "uart.h"
#include "command_decoder.h"
#include "led.h"
#include <LPC21xx.H>

extern struct ReceiverBuffer sReceiverBuffer;
extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern struct TransmitterBuffer sTransmitterBuffer;

int main(){
	
	/* test getchara
	char cTest;
	
	sTransmitterBuffer.cData[0] = 'A';
	sTransmitterBuffer.cData[1] = 'l';
	sTransmitterBuffer.cData[2] = 'a';
	sTransmitterBuffer.cData[3] = 0;
	sTransmitterBuffer.eStatus = BUSY;
	sTransmitterBuffer.ucCharCtr = 0;

	ServoInit(50);
	UART_InitWithInt(9600);

	cTest = Transmitter_GetCharacterFromBuffer();
	cTest = Transmitter_GetCharacterFromBuffer();
	cTest = Transmitter_GetCharacterFromBuffer();
	cTest = Transmitter_GetCharacterFromBuffer();
	cTest = Transmitter_GetCharacterFromBuffer();*/
	
	UART_InitWithInt(9600);

	Transmitter_SendString("test123");
	
	while(1){
		LedOn(1);
		//U0THR = ucCounter;
		//ucCounter
	}
}
