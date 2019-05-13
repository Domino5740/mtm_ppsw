#define HEX_bm 0x000F
#define MAX_TOKEN_NR 3
#define MAX_KEYWORD_STRING_LTH 10

unsigned char ucTokenNr = 0;
typedef enum TokenType {KEYWORD, NUMBER, STRING} TokenType;
typedef enum KeywordCode {LD, ST, RST} KeywordCode;
enum Result {OK, ERROR};
typedef struct Keyword {
	enum KeywordCode eCode;
	char cString[MAX_KEYWORD_STRING_LTH + 1];
} Keyword;
struct Keyword asKeywordList[MAX_TOKEN_NR] = {
	{RST, "reset"},
	{LD, "load"},
	{ST, "store"}
};
typedef union TokenValue {
	enum KeywordCode eKeyword;
	unsigned int uiNumber;
	char * pcString;
} TokenValue;
typedef struct Token {
TokenType eType;
TokenValue uValue;
} Token;
struct Token asToken[MAX_TOKEN_NR];

unsigned char ucFindTokensInString(char *pcString) {
	
	char cCurrentChar;
	unsigned char ucCharCounter = 0;
	unsigned char ucTokenCounter = 0;
	enum TokenState {TOKEN, DELIMITER};
	enum TokenState eTokenState = TOKEN;

	for(ucTokenCounter = 0; ; ucTokenCounter++) {
		
		cCurrentChar = pcString[ucTokenCounter];
		
		switch(eTokenState) {
				case TOKEN:
					if(cCurrentChar==' ') {}
					else if (cCurrentChar=='\0') {
						return ucTokenNr;
					}
					else if(ucTokenNr == MAX_TOKEN_NR) {
						return ucTokenNr;
					}
					else {
						asToken[ucTokenNr].uValue.pcString = pcString + ucTokenCounter;
						ucCharCounter++;
						eTokenState = DELIMITER;
					}
					break;	
				case DELIMITER:
					if(cCurrentChar=='\0') {
						ucTokenNr++;
						return ucTokenNr;
					}
					else if(ucCharCounter == MAX_KEYWORD_STRING_LTH) {
						return ucTokenNr;
					}
					else if(cCurrentChar==' ') {
						ucCharCounter = 0;
						ucTokenNr++;
						eTokenState = TOKEN;
					}
					else {
						--ucTokenCounter;
						eTokenState = TOKEN;
					}
					break;	
		}
	}
}

enum Result eStringToKeyword (char pcStr[], enum KeywordCode *peKeywordCode) {
	for(unsigned char ucTokenCounter=0; ucTokenCounter<MAX_TOKEN_NR; ucTokenCounter++) {
		if(eCompareString(pcStr, asKeywordList[ucTokenCounter].cString) = EQUAL) {
			 *peKeywordCode = asToken[ucTokenCounter].eType;
			return OK;
		}
	}
	return ERROR;
}

void CopyString(char pcSource[], char pcDestination[]) {
	
	unsigned char ucCharCounter;
	
	for(ucCharCounter = 0; pcSource[ucCharCounter]!='\0'; ucCharCounter++) {
		pcDestination[ucCharCounter] = pcSource[ucCharCounter];
	}
	pcDestination[ucCharCounter]='\0';
}

enum CompResult {
	DIFFERENT,
	EQUAL
};

enum CompResult eCompareString(char pcStr1[], char pcStr2[]) {
	
	unsigned char ucCharCounter;
	
	for(ucCharCounter = 0; pcStr1[ucCharCounter] != '\0'; ucCharCounter++) {
		if (pcStr1[ucCharCounter] != pcStr2[ucCharCounter]) {
			return DIFFERENT;
		}
	}
	return EQUAL;
}

void AppendString(char pcSourceStr[], char pcDestinationStr[]) {
	
	unsigned char ucEndPointer;
	
  for(ucEndPointer = 0; pcDestinationStr[ucEndPointer] != '\0' ; ucEndPointer++) {}
  CopyString(pcSourceStr,pcDestinationStr+ucEndPointer);
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
	*puiValue = 0;
	
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


 int main() {
	 ucFindTokensInString("GLODNY JESTEM");
	 
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
