#include <LPC21xx.H>
#include "timer.h"

#define CR_ENABLE_bm 1
#define CR_RESET_bm (1<<1)
#define PCLK_CLOCK 15

void InitTimer0(void) {
	T0TCR = T0TCR | CR_ENABLE_bm;
}

void WaitOnTimer0(unsigned int uiTime) {
	T0TCR = T0TCR | CR_RESET_bm;
	T0TCR = T0TCR & (~CR_RESET_bm);
	while(T0TC != (uiTime * PCLK_CLOCK)) {}
}
