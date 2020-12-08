#include "uart.h"
#include "string.h"
#include <LPC21xx.H>

extern struct ReceiverBuffer sReceiverBuffer;
//extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern struct TransmitterBuffer sTransmitterBuffer;

int main(){
	
	//char cStringToSend[21];
	char cHexStr[20];
	char cStringToSend[21];
	char cStringL[9];
	unsigned int uiCounter;
	UART_InitWithInt(9600);
	
	cStringL[0] = 'l';
	cStringL[1] = 'i';
	cStringL[2] = 'c';
	cStringL[3] = 'z';
	cStringL[4] = 'n';
	cStringL[5] = 'i';
	cStringL[6] = 'k';
	cStringL[7] = ' ';
	cStringL[8] = '\0';
	
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
