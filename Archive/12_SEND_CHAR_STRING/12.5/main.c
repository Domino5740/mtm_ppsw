#include "uart.h"
#include "string.h"
#include "timer_interrupts.h"
#include <LPC21xx.H>

extern struct ReceiverBuffer sReceiverBuffer;
extern struct TransmitterBuffer sTransmitterBuffer;
extern struct Watch sWatch;

int main(){
	
	char cHexStr[20];
	char cStringToSend[21];
	char cSecString[] = "sec ";
	char cMinString[] = "min ";
	UART_InitWithInt(9600);
	
	Timer0Interrupts_Init(1000000, &WatchUpdate);
	
	while(1){
		if(Transmitter_GetStatus() == FREE) {
			if(sWatch.fMinutesValueChanged) {
				sWatch.fMinutesValueChanged = 0;
				CopyString(cMinString, cStringToSend);
				UIntToHexStr(sWatch.ucMinutes, cHexStr);
				AppendString(cHexStr, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
			else if(sWatch.fSecondsValueChanged) {
				sWatch.fSecondsValueChanged = 0;
				CopyString(cSecString, cStringToSend);
				UIntToHexStr(sWatch.ucSeconds, cHexStr);
				AppendString(cHexStr, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
			else {}
		}
	}
}
