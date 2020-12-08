#include <LPC21xx.H>
#include "timer.h"

#define CR_ENABLE_bm 1
#define CR_RESET_bm (1<<1)
#define PCLK_CLOCK 15
#define INTERRUPT_MR0 1
#define RESET_ON_MR0 (1<<1)

void InitTimer0(void) {
	T0TCR = CR_ENABLE_bm | CR_RESET_bm;
	T0TCR = T0TCR & ~CR_RESET_bm;
}

void WaitOnTimer0(unsigned int uiTime) {
	T0TCR = T0TCR | CR_RESET_bm;
	T0TCR = T0TCR & (~CR_RESET_bm);
	while(T0TC < (uiTime * PCLK_CLOCK)) {}
}

void InitTimer0Match0(unsigned int iDelayTime) {
	T0MR0 = (iDelayTime * PCLK_CLOCK);
	T0MCR = INTERRUPT_MR0 | RESET_ON_MR0;
	InitTimer0();
}

void WaitOnTimer0Match0(void) {
	while((T0IR & INTERRUPT_MR0) == 0) { }
	T0IR = INTERRUPT_MR0;
}
