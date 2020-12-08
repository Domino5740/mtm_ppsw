#include "uart.h"
extern struct ReceiverBuffer sReceiverBuffer;

int main (){
	
	Receiver_PutCharacterToBuffer('k');
	Receiver_PutCharacterToBuffer('o');
	Receiver_PutCharacterToBuffer('d');
	Receiver_PutCharacterToBuffer(' ');
	
	sReceiverBuffer.eStatus = EMPTY;
	
	Receiver_PutCharacterToBuffer('k');
	Receiver_PutCharacterToBuffer('o');
	Receiver_PutCharacterToBuffer('d');
	Receiver_PutCharacterToBuffer('1');
	Receiver_PutCharacterToBuffer(' ');
}
