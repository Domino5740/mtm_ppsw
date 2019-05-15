#define HEX_bm 0x000F
#define MAX_TOKEN_NR 4
#define MAX_KEYWORD_NR 4
#define MAX_KEYWORD_STRING_LTH 10

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

void CopyString(char pcSource[], char pcDestination[]) {
	
	unsigned char ucCharCounter;
	
	for(ucCharCounter = 0; pcSource[ucCharCounter]!='\0'; ucCharCounter++) {
		pcDestination[ucCharCounter] = pcSource[ucCharCounter];
	}
	pcDestination[ucCharCounter]='\0';
}

void AppendString(char pcSourceStr[], char pcDestinationStr[]) {
	
	unsigned char ucEndPointer;
	
  for(ucEndPointer = 0; pcDestinationStr[ucEndPointer] != '\0' ; ucEndPointer++) {}
  CopyString(pcSourceStr,pcDestinationStr+ucEndPointer);
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
		if(pcStr[ucCharCounter] >= 'A' & pcStr[ucCharCounter] <= 'F') {
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

	for(ucTokenCounter = 0; ; ucTokenCounter++) {
		
		cCurrentChar = pcString[ucTokenCounter];
		
		switch(eTokenState) {
				case TOKEN:
					if (cCurrentChar==' ') {
						eTokenState = DELIMITER;
					}
					else if (cCurrentChar=='\0') {
						return ucTokenNr;
					}
					else if(ucTokenNr == MAX_TOKEN_NR) {
						return ucTokenNr;
					}
					break;	
				case DELIMITER:
					if(cCurrentChar=='\0') {
						return ucTokenNr;
					}
					else if(cCurrentChar!=' ') {
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

	char cString[] = " 0xA4B5    load word";
	char cNullString[] = " ";

 int main() {
	 DecodeMsg(cString);
	 DecodeMsg(cNullString);
	 
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
