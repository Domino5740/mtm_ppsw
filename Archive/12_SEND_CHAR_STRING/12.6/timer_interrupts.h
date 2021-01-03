__irq void Timer0IRQHandler(void);
void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)());
void WatchUpdate(void);

struct Watch {
	unsigned char ucMinutes;
	unsigned char ucSeconds;
	unsigned char fSecondsValueChanged;
	unsigned char fMinutesValueChanged;
};
