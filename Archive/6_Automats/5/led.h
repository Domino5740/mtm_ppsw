void LedInit(void);
void LedStepRight(void);
void LedStepLeft(void);	
void LedOn(unsigned char ucLedIndeks);

enum eDirection {
	LEFT,
	RIGHT
};

enum LedState {
	LED_NO_STEP,
	LED_STEP_RIGHT
};
