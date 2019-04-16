#include <stdio.h>
void CopyString(char pcSource[], char pcDestination[]) {
    unsigned char ucCounter;
    for(ucCounter = 0; pcDestination[ucCounter]!=0; ucCounter++)
    {
        pcDestination[ucCounter] = pcSource[ucCounter];
    }
}
/*int eCompareString(char pcStr1[], char pcStr2[]) {
    enum CompResult
    {
        DIFFERENT,
        EQUAL
    }
    for(int i = 0; i < 254; i++)
    {
        if (pcStr1[i] == pcStr2[i])
            CompResult = DIFFERENT;
        else
            CompResult = EQUAL;
    }
    return CompResult;
}
int AppendString(char pcSourceStr[], char pcDestinationStr[]) {
    for(i = 0; i < count; i++)
    {
        //kodzik
    }
    
}
*/
int main() {
	char pcTestSource[] = "Test 1";
	char pcTestDest[] = "Test 2";
//	CopyString(pcTestSource,pcTestDest);
	unsigned char i;
	for(i=0;i<4;i++)
	{
        printf("%s", pcTestSource[i]);
  }
}

#define HEX_bm 0x000F

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
		ucCurrentNibble = ((uiValue >> ucNibbleCounter*4) & HEX_bm);
		if(ucCurrentNibble>9) {
			pcStr[5-ucNibbleCounter] = ucCurrentNibble - 10 + 'A';
		}
		else {
			pcStr[5-ucNibbleCounter] = ucCurrentNibble + '0';
		}
		pcStr[6] = '\0';
	}
}

enum Result {
	OK,
	ERROR
};

enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue) {
	
	unsigned char ucCharCounter;
	*puiValue = 0;
	
	if((pcStr[0] != '0') | (pcStr[1] != 'x') | (pcStr[2] == '\0')) {
		return ERROR;
	}
	
	for(ucCharCounter = 2; ucCharCounter<7; ucCharCounter++) {
		if(pcStr[ucCharCounter] == '\0') {
			return OK;
		}
		*puiValue = *puiValue << 4;
		if(pcStr[ucCharCounter] >= 'A') {
			*puiValue = *puiValue | (pcStr[ucCharCounter] - 'A' + 10);
		}
		else {
			*puiValue = *puiValue | (pcStr[ucCharCounter] +'\0');
		}	
	}
	return OK;
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[])
{
	unsigned char ucEndPointer;
	for(ucEndPointer=0;pcDestinationStr[ucEndPointer]!='\0';ucEndPointer++) {}
	UIntToHexStr(uiValue,pcDestinationStr+ucEndPointer);
}
