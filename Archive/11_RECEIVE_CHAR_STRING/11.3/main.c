#include "uart.h"
#include "command_decoder.h"
#include "servo.h"


extern struct ReceiverBuffer sReceiverBuffer;
extern struct Token asToken[MAX_TOKEN_NR];
extern unsigned char ucTokenNr;

int main(){
	
	char cTest[RECEIVER_SIZE];

	ServoInit(50);
	UART_InitWithInt(9600);

	sReceiverBuffer.eStatus = EMPTY;
	
	while(1){
		if(eReceiver_GetStatus() == READY) {
			
			Receiver_GetStringCopy(cTest);
			DecodeMsg(cTest);
			if((ucTokenNr != 0) && (asToken[0].eType == KEYWORD)) {		
				switch(asToken[0].uValue.eKeyword){
					case CALLIB:
						ServoCallib();
					break;
					case GOTO:
						if(asToken[1].eType == NUMBER) {
							ServoGoTo(asToken[1].uValue.uiNumber);
						}
						break;
					default:
					break;
				}
				ucTokenNr=0;
			}
		}
	}
}
