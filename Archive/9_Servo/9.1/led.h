void LedInit(void);
void LedStepRight(void);
void LedStepLeft(void);	
void LedOn(unsigned char ucLedIndeks);

enum eDirection {
	LEFT,
	RIGHT
};

enum LedState {
	STEP_LEFT,
	NO_STEP,
	STEP_RIGHT,
	CALLIB
};
