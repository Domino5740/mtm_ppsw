#include <LPC21xx.H>
#include "timer.h"

#define CR_ENABLE_bm 1
#define CR_RESET_bm (1<<1)
#define PCLK_CLOCK 15
#define INTERRUPT_MR0 1
#define RESET_ON_MR0 (1<<1)

void InitTimer0(void) {
	T0TCR = CR_ENABLE_bm | CR_RESET_bm; //wlaczenie timera i wlaczenie reset
	T0TCR = T0TCR & ~CR_RESET_bm; //wylaczenie reset
}

void WaitOnTimer0(unsigned int uiTime) {
	T0TCR = T0TCR | CR_RESET_bm; //wlaczenie reset
	T0TCR = T0TCR & (~CR_RESET_bm); //wylaczenie reset
	while(T0TC != (uiTime * PCLK_CLOCK)) {} //petla while, warunek - timer < czas podany
} 
//moze nie dzialac poniewaz while i timer maja dwa rozne okresy, timer moze przeskoczyc wartosc czasu podana
//bedzie dzialac gdyby okres while byl mniejszy

void InitTimer0Match0(unsigned int iDelayTime) {
	T0MR0 = (iDelayTime * PCLK_CLOCK); //ustawienie match register0 na odpowiedni czas
	T0MCR = INTERRUPT_MR0 | RESET_ON_MR0; // ustawienie flagi przerwania gdy T0IR == MR0
	InitTimer0();
}

void WaitOnTimer0Match0(void) {
	while((T0IR & INTERRUPT_MR0) == 0) { } //petla, warunek - dopoki nie pojawi sie flaga przerwan na MR0
	T0IR = INTERRUPT_MR0; //wylaczenie flagi przerwania
}
