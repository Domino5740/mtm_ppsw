#include <LPC21xx.H>
#include "led.h"

// TIMER
#define mCOUNTER_ENABLE 1
#define mCOUNTER_RESET  (1<<1)

// CompareMatch
#define mINTERRUPT_ON_MR0 1
#define mRESET_ON_MR0     (1<<1)
#define mMR0_INTERRUPT    1

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_TIMER0_CHANNEL_NR 4

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE (1<<3)

/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer0IRQHandler(){

	T0IR = mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	LedStepRight();		// cos do roboty
	VICVectAddr0=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer0Interrupts_Init(unsigned int uiPeriod){ // microseconds

        // interrupts
	VICIntEnable |= (1 << VIC_TIMER0_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr0  =(unsigned long)Timer0IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 

        // match module

	T0MR0 = 15 * uiPeriod;                 	      // value 
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer

	T0TCR |=  mCOUNTER_ENABLE; // start 

}
/**********************************************/
int main (){
	unsigned int iMainLoopCtr;
	Timer0Interrupts_Init(250000);

	while(1){
	 	iMainLoopCtr++;
	}
}
