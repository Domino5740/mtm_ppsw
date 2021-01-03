#include <LPC21xx.H>
#include "timer_interrupts.h"

#define mCOUNTER_ENABLE 1
#define mCOUNTER_RESET  (1<<1)

#define mINTERRUPT_ON_MR0 1
#define mRESET_ON_MR0     (1<<1)
#define mMR0_INTERRUPT    1

#define VIC_TIMER0_CHANNEL_NR 4

#define mIRQ_SLOT_ENABLE (1<<5)

#define PCLK_CLOCK 15

struct Watch sWatch;

void (*ptrTimer0InterruptFunction)();

__irq void Timer0IRQHandler(){

	T0IR=mMR0_INTERRUPT;
	ptrTimer0InterruptFunction();
	VICVectAddr=0x00;
}

void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)()){

	ptrTimer0InterruptFunction = ptrInterruptFunction;

	VICIntEnable |= (1 << VIC_TIMER0_CHANNEL_NR);
	VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;
	VICVectAddr0  =(unsigned long)Timer0IRQHandler;

	T0MR0 = PCLK_CLOCK * uiPeriod;
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); 

	T0TCR |=  mCOUNTER_ENABLE;

}

void WatchUpdate(void) {
	if(sWatch.ucSeconds < 59) {
		sWatch.ucSeconds++;
		sWatch.fSecondsValueChanged = 1;
	}
	else {
		sWatch.ucSeconds = 0;
		sWatch.ucMinutes++;
		sWatch.fSecondsValueChanged = 1;
		sWatch.fMinutesValueChanged = 1;
	}
}
