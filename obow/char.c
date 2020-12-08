#define HEX_bm 0x000F
#define MAX_TOKEN_NR 4
#define MAX_KEYWORD_NR 4
#define MAX_KEYWORD_STRING_LTH 10
#include <stdio.h>

enum CompResult {DIFFERENT, EQUAL};
enum Result {OK, ERROR};

unsigned char ucTokenNr = 0;
typedef enum TokenType {KEYWORD, NUMBER, STRING} TokenType;
typedef enum KeywordCode {LD, ST, RST, DEL} KeywordCode;

typedef struct Keyword {
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
} Keyword;

typedef union TokenValue {
	enum KeywordCode eKeyword;
	unsigned int uiNumber;
	char * pcString;
} TokenValue;

typedef struct Token {
enum TokenType eType;
union TokenValue uValue;
} Token;

struct Token asToken[MAX_TOKEN_NR];

struct Keyword asKeywordList[MAX_TOKEN_NR] = {
	{RST, "reset"},
	{LD, "load"},
	{ST, "store"},
	{DEL, "delete"}
};

void CopyString(char pcTokens[], char pcDestination[]) {

	unsigned char ucCharCounter;

	for(ucCharCounter = 0; pcTokens[ucCharCounter] != '\0'; ucCharCounter++) {
		pcDestination[ucCharCounter] = pcTokens[ucCharCounter];
	}
	while (pcDestination[ucCharCounter] != '\0') {
		pcDestination[ucCharCounter] = '\0';
		ucCharCounter++;
	}
}

void AppendString(char pcTokensStr[], char pcDestinationStr[]) {

	unsigned char ucEndPointer;

  for(ucEndPointer = 0; pcDestinationStr[ucEndPointer] != '\0' ; ucEndPointer++) {}
  CopyString(pcTokensStr,pcDestinationStr+ucEndPointer);
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[]) {

	unsigned char ucCharCounter;

	for(ucCharCounter = 0; pcStr1[ucCharCounter] != '\0'; ucCharCounter++) {
		if (pcStr1[ucCharCounter] != pcStr2[ucCharCounter]) {
			return DIFFERENT;
		}
	}
	return EQUAL;
}

void ReplaceCharactersInString(char pcString[], char cOldChar, char cNewChar) {

	unsigned char ucCharCounter;

	for(ucCharCounter = 0; pcString[ucCharCounter] != '\0'; ucCharCounter++) {
		if(pcString[ucCharCounter] == cOldChar) {
			pcString[ucCharCounter] = cNewChar;
		}
	}
}

void UIntToHexStr(unsigned int uiValue, char pcStr[]) {

	unsigned char ucNibbleCounter;
	unsigned char ucCurrentNibble;

	pcStr[0]='0';
	pcStr[1]='x';

	for(ucNibbleCounter=0; ucNibbleCounter<4; ucNibbleCounter++) {
		ucCurrentNibble = ((uiValue >> (ucNibbleCounter*4)) & HEX_bm);
		if(ucCurrentNibble>9) {
			pcStr[5-ucNibbleCounter] = (ucCurrentNibble - 10 + 'A');
		}
		else {
			pcStr[5-ucNibbleCounter] = (ucCurrentNibble + '0');
		}
		pcStr[6] = '\0';
	}
}

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue) {

	unsigned char ucCharCounter;

	if((pcStr[0] != '0') | (pcStr[1] != 'x') | (pcStr[2] == '\0')) {
		return ERROR;
	}

	for(ucCharCounter=2; pcStr[ucCharCounter] != '\0'; ucCharCounter++) {
		if(ucCharCounter > 6) {
			return ERROR;
		}
	  *puiValue = *puiValue << 4;
		if((pcStr[ucCharCounter] >= 'A') & (pcStr[ucCharCounter] <= 'F')) {
			*puiValue = *puiValue | (pcStr[ucCharCounter] - 'A' + 10);
		}
		else if(pcStr[ucCharCounter] > 'F') {
			return ERROR;
		}
		else {
			*puiValue = *puiValue | (pcStr[ucCharCounter] - '0');
		}
	}
	return OK;
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]) {
	unsigned char ucEndPointer;
	for(ucEndPointer=0; pcDestinationStr[ucEndPointer] != '\0'; ucEndPointer++) {}
	UIntToHexStr(uiValue,pcDestinationStr+ucEndPointer);
}

unsigned char ucFindTokensInString(char *pcString) {

	char cCurrentChar;
	unsigned char ucTokenCounter = 0;
	enum TokenState {TOKEN, DELIMITER};
	enum TokenState eTokenState = DELIMITER;
	ucTokenNr = 0;


	for(ucTokenCounter = 0; ; ucTokenCounter++) {

		cCurrentChar = pcString[ucTokenCounter];

		switch(eTokenState) {
				case TOKEN:
					if (cCurrentChar == ' ') {
						eTokenState = DELIMITER;
					}
					else if (cCurrentChar == '\0') {
						return ucTokenNr;
					}
					else if(ucTokenNr == MAX_TOKEN_NR) {
						return ucTokenNr;
					}
					break;
				case DELIMITER:
					if(cCurrentChar== '\0') {
						return ucTokenNr;
					}
					else if(cCurrentChar != ' ') {
						eTokenState = TOKEN;
						asToken[ucTokenNr].uValue.pcString = pcString + ucTokenCounter;
						ucTokenNr++;
					}
					break;
		}
	}
}

enum Result eStringToKeyword (char pcStr[], enum KeywordCode *peKeywordCode) {

	unsigned char ucKeywordCounter;

	for(ucKeywordCounter = 0; ucKeywordCounter<MAX_KEYWORD_NR; ucKeywordCounter++) {
		if(eCompareString(pcStr, asKeywordList[ucKeywordCounter].cString) == EQUAL) {
			*peKeywordCode = asKeywordList[ucKeywordCounter].eCode;
			return OK;
		}
	}
	return ERROR;
}

void DecodeTokens(void) {

	unsigned char ucTokenCounter;

	for(ucTokenCounter = 0; ucTokenCounter < ucTokenNr; ucTokenCounter++) {
		if(eStringToKeyword(asToken[ucTokenCounter].uValue.pcString, &asToken[ucTokenCounter].uValue.eKeyword) == OK) {
			asToken[ucTokenCounter].eType = KEYWORD;
		}
		else if(eHexStringToUInt(asToken[ucTokenCounter].uValue.pcString, &asToken[ucTokenCounter].uValue.uiNumber) == OK) {
			asToken[ucTokenCounter].eType = NUMBER;
		}
		else {
			asToken[ucTokenCounter].eType = STRING;
			asToken[ucTokenCounter].uValue.pcString = asToken[ucTokenCounter].uValue.pcString;
		}
	}
}

void DecodeMsg(char *pcString) {
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', '\0');
	DecodeTokens();
}

void TestOf_CopyString(void) {

	char cSource[] = "Testowy string";
	char cDestination[] = "asafsfafsfsfsffssfsa";

	printf("CopyString\n\n ");
	printf ("Test 1 - ");
	// sprawdzenie czy kopiowanie string�w dzia�a w�a�ciwie
	CopyString(cSource, cDestination);
	if (eCompareString(cSource, cDestination) == EQUAL) {
		printf("OK\n\n\n");
		}
	else {
		printf("Error\n\n\n");
		}
}

void TestOf_eCompareString(void) {

	char cEqualString1[] = "Testowy string";
	char cEqualString2[] = "Testowy string";
	char cNotEqualString[] = "Test";

	printf("eCompareString\n\n ");
	printf ("Test 2 - ");
	// sprawdzenie czy por�wnanie takich samych string�w da wynik EQUAL
	if (eCompareString(cEqualString1, cEqualString2) == EQUAL) {
		printf("OK\n ");
		}
	else {
		printf("Error\n ");
	}
	printf ("Test 3 - ");
	//sprawdzenie czy dwa r�ne stringi b�d� uznane przez funkcje za r�ne
	if(eCompareString(cEqualString1, cNotEqualString) == DIFFERENT) {
		 printf("OK\n\n\n");
	}
	else {
		printf("Error\n\n\n");
	}
}

void TestOf_AppendString(void) {

	char cSource[] = "Testowy string";
	char cDestination[] = "tosty ";
	char cTest[] = "tosty Testowy string";

	printf("AppendString\n\n ");
	printf ("Test 4 - ");
	// sprawdzenie czy dodawanaie na koniec stringa dzia�a w�a�ciwie
	AppendString(cSource, cDestination);
	if (eCompareString(cTest, cDestination) == EQUAL) {
		printf("OK\n\n\n");
		}
	else {
		printf("Error\n\n\n");
		}
}

void TestOf_ReplaceCharactersInString(void) {

	char cString[] = "Testowy string test";
	char cOldChar = 'e';
	char cNewChar = 'o';
	char cNewString[] = "Tostowy string tost";

	printf("ReplaceCharactersInString\n\n ");
	printf ("Test 5 - ");
	// sprawdzenie czy znaki s� poprawnie zamieniane
	ReplaceCharactersInString(cString, cOldChar, cNewChar);
	if (eCompareString(cString, cNewString) == EQUAL) {
		printf("OK\n\n\n");
	}
	else {
		printf("Error\n\n\n");
	}
}

void TestOf_UIntToHexStr(void) {

	unsigned int uiTest = 21157;
	char cTestHex[] = "0x52A5";
	char cString[] = "sfsfsfsffs";

	printf("UIntToHexStr\n\n ");
	printf ("Test 6 - ");
	// sprawdzenie czy liczba poprawnie zamieniana na zapis heksadecymalny
	UIntToHexStr(uiTest, cString);
	if (eCompareString(cString, cTestHex) == EQUAL) {
		printf("OK\n\n\n");
	 }
	 else {
		 printf("Error\n\n\n");
	 }
}

void TestOf_eHexStringToUInt(void) {

	unsigned int uiTest = 2153353;
	char cTestHex[] = "0x52A5";
	char cTestNullHex[] = "0x";
	char cTestNo0xHex[] = "52A";
	char cTestTooLongHex[] = "0x2542A6D27";
	enum Result eResult;

	printf("eHexStringToUInt\n\n ");
	printf ("Test 7 - ");
	// sprawdzenie czy liczba poprawnie zamieniana na zapis dziesi�tny unsigned int
	eResult = eHexStringToUInt(cTestHex, &uiTest);
	if (eResult == OK) {
		printf("OK\n ");
	}
	else {
		printf("Error\n ");
	}
	printf ("Test 8 - ");
	// sprawdzenie czy liczba pusta zostanie zamieniona
	eResult = eHexStringToUInt(cTestNullHex, &uiTest);
	if (eResult == ERROR) {
		printf("OK\n ");
	}
	else {
		printf("Error\n ");
	}
	printf ("Test 9 - ");
	// sprawdzenie czy liczba bez 0x zostanie zamieniona
	eResult = eHexStringToUInt(cTestNo0xHex, &uiTest);
	if (eResult == ERROR) {
		printf("OK\n ");
	}
	else {
		printf("Error\n ");
	}
	printf ("Test 10 - ");
	// sprawdzenie czy zbyt d�ugi string zostanie zamieniony
	eResult = eHexStringToUInt(cTestTooLongHex, &uiTest);
	if (eResult == ERROR) {
		printf("OK\n\n\n");
	}
	else {
		printf("Error\n\n\n");
	}
}

void TestOf_AppendUIntToString(void) {

	unsigned int uiTest = 21157;
	char cString[] = "Test";
	char cEndString[] = "Test0x52A5";

	printf("AppendUIntToString\n\n ");
	printf ("Test 11 - ");
	// sprawdzenie czy liczba poprawnie dodawana na koniec stringa
	AppendUIntToString(uiTest, cString);
	if (eCompareString(cString, cEndString) == EQUAL) {
		printf("OK\n\n\n");
	}
	else {
		printf("Error\n\n\n");
	}
}

void TestOf_ucFindTokensInString(void) {

	unsigned char ucTokenNumber;
	char cNullToken[] = "";
	char cTokenMoreSpaces[] = "Test  test";
	char cTokens[] = " elo reset pyyk";

	printf("ucFindTokensInString\n\n ");
	printf ("Test 12 - ");
	// sprawdzenie czy nie odnaleziono token�w w przypadku pustego stringa
	ucTokenNumber = ucFindTokensInString(cNullToken);
	if (ucTokenNumber != 0) {
		printf("Error\n ");
	 }
	else if(asToken[0].uValue.pcString != '\0') {
		printf("Error\n ");
	}
	else {
		printf("OK\n ");
	}
	printf ("Test 13 - ");
	// sprawdzenie czy odnaleziono odpowiedni� liczb� token�w(kilka spacji mi�dzy dwoma tokenami)
	ucTokenNumber = ucFindTokensInString(cTokenMoreSpaces);
	if (ucTokenNumber != 2) {
		printf("Error\n ");
	}
	else {
		printf("OK\n ");
	}
	printf ("Test 14 - ");
	// sprawdzenie czy odnaleziono odpowiedni� liczb� token�w(spacja na pocz�tku, 3 tokeny)
	ucTokenNumber = ucFindTokensInString(cTokens);
	if(ucTokenNumber != 3) {
		printf("Error\n\n\n");
	 }
	 else {
		 printf("OK\n\n\n");
	 }
}

void TestOf_eStringToKeyword(void) {

	enum KeywordCode eTokenCode;
	char cResetToken[] = "reset";
	char cLoadToken[] = "load";
	char cNotKeywordToken[] = "roset";

	printf("eStringToKeyword\n\n ");
	printf ("Test 15 - ");
	// sprawdzenie czy odczytano token reset
	if(eStringToKeyword(cResetToken, &eTokenCode) == OK) {
		printf("OK\n ");
	}
	else {
		printf("ERROR\n ");
	}
	printf ("Test 16 - ");
	// sprawdzenie czy odczytano load
	if(eStringToKeyword(cLoadToken, &eTokenCode) == OK) {
		printf("OK\n ");
	}
	else {
		printf("ERROR\n ");
	}
	printf ("Test 17 - ");
	// sprawdzenie czy nie odczytano b��dnego keyworda
	if(eStringToKeyword(cNotKeywordToken, &eTokenCode) == ERROR) {
		printf("OK\n\n\n");
	}
	else {
		printf("ERROR\n\n\n");
	}
}

void TestOf_DecodeTokens() {

	//unsigned char ucTokenNumber;
	char pcTokens[]="0xA318 iledalbym store";

	printf("DecodeTokens\n\n ");
	printf("Test 18 - ");
	//sprawdzenie czy komunikat jest odkodowany poprawnie (liczba i jej wartosc, string wska�nik na niego,
	//slowo kluczowe i jego kod)
	ucFindTokensInString(pcTokens);
	ReplaceCharactersInString(pcTokens,' ', NULL);
	DecodeTokens();
	if ((asToken[0].eType == NUMBER) & (asToken[0].uValue.uiNumber==0xA318) &
		(asToken[1].eType == STRING) & (&pcTokens[7] == asToken[1].uValue.pcString) &
		(asToken[2].eType == KEYWORD) & (asToken[2].uValue.eKeyword==ST)) {
		printf("OK\n\n\n");
	}
	else {
		printf("ERROR\n\n\n");
	}
}

void TestOf_DecodeMsg() {

	char pcTokens[]="0xA4B5 quattroADR load";

	printf("DecodeMsg\n\n ");
	printf("Test 19 - ");
	//test analogiczny do poprzedniego
	DecodeMsg(pcTokens);
	if ((asToken[0].eType==NUMBER) & (asToken[0].uValue.uiNumber==0xA4B5) &
		(asToken[1].eType==STRING) & (&pcTokens[7] == asToken[1].uValue.pcString) &
		(asToken[2].eType==KEYWORD) & (asToken[2].uValue.eKeyword==LD)) {
		printf("OK\n\n\n");
	}
	else {
		printf("ERROR\n\n\n");
	}
}

 int main() {

	printf("TESTY FUNKCJI - LANCUCHY ZNAKOWE - OPERACJE PROSTE\n\n\n");

	TestOf_CopyString();
  	TestOf_eCompareString();
	TestOf_AppendString();
	TestOf_ReplaceCharactersInString();

	printf("TESTY FUNKCJI - LANCUCHY ZNAKOWE - KONWERSJE\n\n\n");

	TestOf_UIntToHexStr();
	TestOf_eHexStringToUInt();
	TestOf_AppendUIntToString();

	printf("TESTY FUNKCJI - DEKODOWANIE KOMUNIKATOW\n\n\n");

	TestOf_ucFindTokensInString();
	TestOf_eStringToKeyword();
	TestOf_DecodeTokens();
	TestOf_DecodeMsg();

//	 DecodeMsg(cString);
//	 DecodeMsg(cNullString);

	/* test konwersji
	volatile enum Result TestGoodString;
	volatile enum Result TestTooLongString;
	volatile enum Result TestNullString;
	volatile enum Result TestNotHexString;
	volatile enum Result TestBadString;
	unsigned int uiReturnedValue;
	char cString[]="2137223";

	//UIntToHexStr(65000, cString);
	TestGoodString = eHexStringToUInt("0x2115", &uiReturnedValue);
	UIntToHexStr(uiReturnedValue, cString);
	TestTooLongString = eHexStringToUInt("0x2115FFFF", &uiReturnedValue);
	TestNullString = eHexStringToUInt("0x", &uiReturnedValue);
	TestNotHexString = eHexStringToUInt("0xGG", &uiReturnedValue);
	TestBadString = eHexStringToUInt("2115", &uiReturnedValue);
	*/
}
