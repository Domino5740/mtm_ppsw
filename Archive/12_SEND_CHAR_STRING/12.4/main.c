#include "uart.h"
#include "string.h"
#include <LPC21xx.H>

extern struct ReceiverBuffer sReceiverBuffer;
//extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern struct TransmitterBuffer sTransmitterBuffer;

int main(){
	
	char cHexStr[20];
	char cStringToSend[21];
	unsigned int uiCounter;
	char cStringL[] = "licznik ";
	UART_InitWithInt(9600);
	
	
	
	while(1){
		if(sTransmitterBuffer.eStatus == FREE) {
			CopyString(cStringL, cStringToSend);
			UIntToHexStr(uiCounter, cHexStr);
			AppendString(cHexStr, cStringToSend);
			Transmitter_SendString(cStringToSend);
			uiCounter++;
		}
	}
}
