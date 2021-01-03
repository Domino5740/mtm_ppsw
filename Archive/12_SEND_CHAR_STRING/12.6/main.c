#include "uart.h"
#include "string.h"
#include "command_decoder.h"
#include "timer_interrupts.h"
#include <LPC21xx.H>

extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;
extern struct Watch sWatch;

int main(){
	
	char cHexStr[TRANSMITTER_SIZE - 1];
	char cStringToSend[TRANSMITTER_SIZE];
	char cReceivedMessage[RECEIVER_SIZE];
	unsigned int uiCalcResult = 0;
	char fSendID = 0;
	char fSendCalc = 0;
	char fSendUnknown = 0;
	
	UART_InitWithInt(9600);
	Timer0Interrupts_Init(1000000, &WatchUpdate);
	
	while(1) {
		
		if (Receiver_GetStatus() == READY) {
			Receiver_GetStringCopy(cReceivedMessage);
			DecodeMsg(cReceivedMessage);
		
			if(asToken[0].eType == KEYWORD && ucTokenNr > 0) {
				switch(asToken[0].uValue.eKeyword) {
					case ID:
						fSendID = 1;
						break;
					case CALC:
						if(asToken[1].eType == NUMBER) {
							uiCalcResult = (asToken[1].uValue.uiNumber) * 2;
							fSendCalc = 1;
						}
						else {
							fSendUnknown = 1;
						}
						break;
				}
			}
			else {
				fSendUnknown = 1;
			}
		}
		
		if(Transmitter_GetStatus() == FREE) {
			if(sWatch.fMinutesValueChanged) {
				sWatch.fMinutesValueChanged = 0;
				CopyString("min ", cStringToSend);
				UIntToHexStr(sWatch.ucMinutes, cHexStr);
				AppendString(cHexStr, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
			else if(sWatch.fSecondsValueChanged) {
				sWatch.fSecondsValueChanged = 0;
				CopyString("sec ", cStringToSend);
				UIntToHexStr(sWatch.ucSeconds, cHexStr);
				AppendString(cHexStr, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
			else if(fSendID) {
				fSendID = 0;
				Transmitter_SendString("id LPC2132");
			}
			else if(fSendCalc) {
				fSendCalc = 0;
				CopyString("calc ", cStringToSend);
				UIntToHexStr(uiCalcResult, cHexStr);
				AppendString(cHexStr, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
			else if(fSendUnknown) {
				fSendUnknown = 0;
				Transmitter_SendString("Unknown Command!");
			}
		}
	}
}
