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
	char *pcString;
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

	for(ucCharCounter = 0; pcTokens[ucCharCounter]!='\0'; ucCharCounter++) {
		pcDestination[ucCharCounter] = pcTokens[ucCharCounter];
	}
	pcDestination[ucCharCounter]='\0';
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
    *puiValue = 0;
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
	ucTokenNr = 0;
	unsigned char ucTokenCounter = 0;
	enum TokenState {TOKEN, DELIMITER};
	enum TokenState eTokenState = DELIMITER;

	for(ucTokenCounter = 0; ; ucTokenCounter++) {

		cCurrentChar = pcString[ucTokenCounter];

		switch(eTokenState) {
				case TOKEN:
					if (cCurrentChar == '\0') {
						return ucTokenNr;
					}
					else if(ucTokenNr == MAX_TOKEN_NR) {
						return ucTokenNr;
					}
					else if (cCurrentChar == ' ') {
						eTokenState = DELIMITER;
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
		}
	}
}

void DecodeMsg(char *pcString) {
	ucTokenNr = ucFindTokensInString(pcString);
	ReplaceCharactersInString(pcString, ' ', '\0');
	DecodeTokens();
}

void TestOf_CopyString(void) {

	char cDestination[] = "asafsfafsfsfsffssfsa";

	printf("CopyString\n\n ");
	printf ("Test 1 - ");
	//pusty string
	CopyString("", cDestination);
	if (eCompareString("", cDestination) == EQUAL) printf("OK\n\n\n"); else printf("Error\n\n\n");
}

void TestOf_eCompareString(void) {

	printf("eCompareString\n\n ");
	printf ("Test 2 - ");
	//takie same stringi
	if (eCompareString("Testowy string", "Testowy string") == EQUAL) printf("OK\n "); else printf("Error\n ");
	printf ("Test 3 - ");
	//różne stringi
	if(eCompareString("Test1", "Test2") == DIFFERENT) printf("OK\n "); else printf("Error\n ");
	printf ("Test 4 - ");
	//puste stringi
	if(eCompareString("", "") == EQUAL) printf("OK\n\n\n"); else printf("Error\n\n\n");
}

void TestOf_AppendString(void) {

	char cDestination[] = "tosty ";

	printf("AppendString\n\n ");
	printf ("Test 5 - ");
	//pusty string
	AppendString("", cDestination);
	if (eCompareString("tosty ", cDestination) == EQUAL) printf("OK\n\n\n"); else printf("Error\n\n\n");
}

void TestOf_ReplaceCharactersInString(void) {

	char cString[] = "Testowy string test";
	printf("ReplaceCharactersInString\n\n ");
	printf ("Test 6 - ");
	//test do tokenow (spacje na NULLe)
	ReplaceCharactersInString(cString, ' ', '\0');
	if (eCompareString(cString, "Testowy\0string\0test") == EQUAL) printf("OK\n\n\n"); else printf("Error\n\n\n");
}

void TestOf_UIntToHexStr(void) {

	char cString[] = "sfsfsfsffs";

	printf("UIntToHexStr\n\n ");
	printf ("Test 7 - ");
	//wartosci na krancach przedzialu dzialaniu funkcji
	UIntToHexStr(43510, cString);
	if (eCompareString(cString, "0xA9F6") == EQUAL) printf("OK\n\n\n"); else printf("Error\n\n\n");
}

void TestOf_eHexStringToUInt(void) {

	unsigned int uiTest = 2153353;
	enum Result eResult;

	printf("eHexStringToUInt\n\n ");
	printf ("Test 8 - ");
	//zwykla liczba
	eResult = eHexStringToUInt("0x52A5", &uiTest);
	if (eResult == OK) printf("OK\n "); else printf("Error\n ");
	printf ("Test 9 - ");
	//null string
	eResult = eHexStringToUInt("0x", &uiTest);
	if (eResult == ERROR) printf("OK\n "); else printf("Error\n ");
	printf ("Test 10 - ");
	//string bez 0x
	eResult = eHexStringToUInt("52A", &uiTest);
	if (eResult == ERROR) printf("OK\n "); else printf("Error\n ");
	printf ("Test 11 - ");
	//zbyt dlugi string
	eResult = eHexStringToUInt("0x2524242ASDF", &uiTest);
	if (eResult == ERROR) printf("OK\n\n\n"); else printf("Error\n\n\n");
}

void TestOf_AppendUIntToString(void) {

	char cString[] = "Test";

	printf("AppendUIntToString\n\n ");
	printf ("Test 12 - ");
	//liczba na koniec stringa
	AppendUIntToString(21157, cString);
	if (eCompareString(cString, "Test0x52A5") == EQUAL) printf("OK\n\n\n"); else printf("Error\n\n\n");
}

void TestOf_ucFindTokensInString(void) {

	unsigned char ucTokenNumber;
	char cNullToken[] = "";
	char cTokenMoreSpaces[] = "Test  test";
	char cTokens[] = " heh reset 0xA6";
	char cTooMuchTokens[] = "raz dwa 3 4 5";

	printf("ucFindTokensInString\n\n ");
	printf ("Test 13 - ");
	//pusty string
	ucTokenNumber = ucFindTokensInString(cNullToken);
	if ((ucTokenNumber != 0) | (asToken[0].uValue.pcString != '\0')) printf("Error\n "); else printf("OK\n ");
	printf ("Test 14 - ");
	//kilka spacji miedzy tokenami
	ucTokenNumber = ucFindTokensInString(cTokenMoreSpaces);
	if ((ucTokenNumber != 2) | (asToken[0].uValue.pcString != &cTokenMoreSpaces[0]) |
        (asToken[1].uValue.pcString != &cTokenMoreSpaces[6])) printf("Error\n ");
	else printf("OK\n ");
    printf ("Test 15 - ");
	//spacja na poczatku
	ucTokenNumber = ucFindTokensInString(cTokens);
	if((ucTokenNumber != 3) | (asToken[0].uValue.pcString != &cTokens[1]) | (asToken[1].uValue.pcString != &cTokens[5]) |
    (asToken[2].uValue.pcString != &cTokens[11])) printf("Error\n "); else printf("OK\n ");
    printf ("Test 16 - ");
    //za duzo tokenow
    ucTokenNumber = ucFindTokensInString(cTooMuchTokens);
    if((ucTokenNumber != 4) | (asToken[0].uValue.pcString != &cTooMuchTokens[0]) |
       (asToken[1].uValue.pcString != &cTooMuchTokens[4]) | (asToken[2].uValue.pcString != &cTooMuchTokens[8]) |
        (asToken[3].uValue.pcString != &cTooMuchTokens[10])) printf("Error\n\n\n"); else printf("OK\n\n\n");
}

void TestOf_eStringToKeyword(void) {

	enum KeywordCode eTokenCode;
	printf("eStringToKeyword\n\n ");
	printf ("Test 17 - ");
	//token reset
	if((eStringToKeyword("reset", &eTokenCode) == OK) & (eTokenCode == RST)) printf("OK\n "); else printf("ERROR\n ");
	printf ("Test 18 - ");
	//slowo nie bedace keywordem
	if(eStringToKeyword("lada", &eTokenCode) == ERROR) printf("OK\n\n\n"); else printf("ERROR\n\n\n");
}

void TestOf_DecodeTokens() {

	asToken[0].uValue.pcString = "0xA318";
	asToken[1].uValue.pcString = "iledalbym";
    asToken[2].uValue.pcString = "store";

	printf("DecodeTokens\n\n ");
	printf("Test 19 - ");
	//number, string, keyword
	DecodeTokens();
	if ((asToken[0].eType == NUMBER) & (asToken[0].uValue.uiNumber == 0xA318) &
        (asToken[1].eType == STRING) & (&"iledalbym" == asToken[1].uValue.pcString) &
		(asToken[2].eType == KEYWORD) & (asToken[2].uValue.eKeyword == ST)) printf("OK\n\n\n");
    else printf("ERROR\n\n\n");
}

void TestOf_DecodeMsg() {

	char pcTokens[]="0xA4B5 quattroADR load";

	printf("DecodeMsg\n\n ");
	printf("Test 20 - ");
	//number, string, keyword
	DecodeMsg(pcTokens);
	if ((asToken[0].eType==NUMBER) & (asToken[0].uValue.uiNumber == 0xA4B5) &
		(asToken[1].eType==STRING) & (&pcTokens[7] == asToken[1].uValue.pcString) &
		(asToken[2].eType==KEYWORD) & (asToken[2].uValue.eKeyword==LD))	printf("OK\n\n\n");
    else printf("ERROR\n\n\n");
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
}
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
