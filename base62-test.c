
#include "base62-test.h"

//##############################################################################

void test_hex_print(const unsigned char* string, int len) {
	printf("[%d] : ", len);
	for (int i=0; i<len; i++) {
		printf("%02x ", (string[i]&0xff));
	}
}

//##############################################################################

const char Map10To62[62] = { 
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' 
};

const char test_string[5] = {'z', 'a', 0 };

typedef unsigned char BYTE;
int GetMSD(const BYTE* bytes, int max) {
	return 1;
	int ret = max;
	while (ret>0) {
		if (bytes[ret - 1]!=0) break;
		ret--;
	}
	if (ret==0) ret = -1;
	return ret-1;
}

void test_decode() {
	
	BYTE base62_string[7] = {0,0,0,0,0,0,0};
	
	printf("test_string %02x %02x %02x %02x ...\n", test_string[0], test_string[1], test_string[2], test_string[3]);
	
	int iSrcValMaxCount = strlen(test_string);
	printf("iSrcValMaxCount %i ...\n", iSrcValMaxCount);
	
	
	BYTE pcCarries[iSrcValMaxCount];
    memcpy(pcCarries, &test_string, iSrcValMaxCount);
	printf("pcCarries %02x %02x ...\n", pcCarries[0], pcCarries[1]);
	
	bool bCarry = false;
	int iDstDigitCount = 0;
	do {
		
		int iMSDPos = GetMSD(pcCarries, iSrcValMaxCount);
		printf("iMSDPos %i ...\n", iMSDPos);
		
		if (0 > iMSDPos) break;
		
		bCarry = false;
		
		int iDstDigit = 0;
		for (int i = iMSDPos; i >= 0; i--) {
			
			int iSrcDigit = pcCarries[i];
			iDstDigit = (iDstDigit * 256 + iSrcDigit);
			
			printf("i %i c %d s %04x d %04x...\n", i, bCarry, iSrcDigit, iDstDigit);
			if (iDstDigit >= 62) {
				pcCarries[i] = ((iDstDigit / 62) & 0xFF);
				iDstDigit %= 62;
				bCarry = true;
				printf("%02x %02x...\n", ((iDstDigit / 62) & 0xFF), ((iDstDigit % 62) & 0xFF));
			}
			else {
				pcCarries[i] = 0;
			}
			printf("pcCarries %02x %02x ...\n", pcCarries[0], pcCarries[1]);
         }
		
		char result = Map10To62[iDstDigit];
		base62_string[iDstDigitCount] = result;
		
		printf("pcCarries %02x %02x ...\n", pcCarries[0], pcCarries[1]);
		printf("base62_string %02x %02x %02x %02x %02x %02x ...\n", base62_string[0], base62_string[1], base62_string[2], base62_string[3], base62_string[4], base62_string[5]);
		
		iDstDigitCount++;
		
	} while(bCarry);
	
	printf("base62_string %02x %02x %02x %02x %02x %02x ...\n", base62_string[0], base62_string[1], base62_string[2], base62_string[3], base62_string[4], base62_string[5]);
}

typedef unsigned char byte;
static const byte Map62To10[75] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
	127, 127, 127, 127, 127, 127, 127,
	10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 
	127, 127, 127, 127, 127, 127, 
	36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61};
int GetMSB(const byte* data, int max) {
	int i=0;
	while (i<max-1) {
		if (data[i]==0x00) break;
		i++;
	}
	return i;
}
bool base62_encode(const byte* src, int srcLen, char** dst, int* dstLen) {
	
	char* out = dst;
	
	printf("src:");
	test_hex_print(src, srcLen);
	printf("\n");
	
	byte scratchPad[srcLen];
    memcpy(scratchPad, src, srcLen);
	
	printf("scratchPad:");
	test_hex_print(scratchPad, srcLen);
	printf("\n");
	
	int iDstDigitCount = 0;
	
	bool bCarry = false;
	bool flagCarry = true;
	do {
		
		int iMSDPos = GetMSB(scratchPad, srcLen);
		printf("input to run: %i\n", iMSDPos);
		
		printf("scratchPad:");
		test_hex_print(scratchPad, srcLen);
		printf("\n");
		
		if (0 > iMSDPos) break;
		
		bCarry = false;
		
		int iDstDigit = 0;
		for (int i = iMSDPos; i >= 0; i--) {
			
			int iSrcDigit = scratchPad[i];
			iDstDigit = (iDstDigit * 256 + iSrcDigit);
			
			if (iDstDigit >= 62) {
				scratchPad[i] = ((iDstDigit / 62) & 0xFF);
				iDstDigit %= 62;
				bCarry = true;
			}
			else {
				scratchPad[i] = 0;
			}
         }
		
		char result = Map10To62[iDstDigit];	
		printf("result %02x ...\n", result);
		out[iDstDigitCount] = result;
		
		iDstDigitCount++;
		
	} while(bCarry);
	*dstLen = iDstDigitCount;
	return true;
}
bool base62_decode(const char* src, int srcLen, byte** dst, int* dstLen) {
	
	byte* out = dst;
	
	printf("src:");
	test_hex_print((const unsigned char *)src, srcLen);
	printf("\n");
	
	char scratchPad[srcLen];
    memcpy(scratchPad, src, srcLen);
	
	printf("scratchPad:");
	test_hex_print((const unsigned char *)scratchPad, srcLen);
	printf("\n");
	
	int iDstDigitCount = 0;
	
	bool bCarry = false;
	bool flagCarry = true;
	do {
		
		//int iMSDPos = GetMSB(scratchPad, srcLen);
		int iMSDPos = 0;
		printf("input to run: %i\n", iMSDPos);
		
		printf("scratchPad:");
		test_hex_print(scratchPad, srcLen);
		printf("\n");
		
		if (0 > iMSDPos) break;
		
		bCarry = false;
		
		int iDstDigit = 0;
		for (int i = iMSDPos; i < srcLen; i++) {
			
			int iMappingPos = scratchPad[i] - 0x30;
			if ((iMappingPos < 0) || (iMappingPos > 74) || (Map62To10[iMappingPos] == 127))
            {
                   return false;
            }
			int iSrcDigit = Map62To10[iMappingPos];
			iDstDigit = (iDstDigit * 62 + iSrcDigit);
			
			if (iDstDigit >= 256) {
				scratchPad[i] = Map10To62[iDstDigit / 256];
				iDstDigit %= 256;
				bCarry = true;
			}
			else {
				scratchPad[i] = 0x30;
			}
			
			printf("scratchPad:");
			test_hex_print(scratchPad, srcLen);
			printf("\n");

         }
		
		char result = iDstDigit;	
		printf("result %02x ...\n", result);
		out[iDstDigitCount] = result;
		
		iDstDigitCount++;
		
	} while(bCarry);
	*dstLen = iDstDigitCount;
	return true;
}

//##############################################################################

int main (int argc, char *argv[]) {
	//test_decode();
	
	const byte test_string1[2] = { 'z', 'a' };
	printf("test_string1:");
	test_hex_print(test_string1, 2);
	printf("\n");
	
	int cnt = 0;
	char result[256];
	//result = (char *)malloc(sizeof(byte)*256);
    memset(&result, 0, 256);

	base62_encode(test_string1, 2, (char **)&result, &cnt);
	
	printf("result:");
	test_hex_print((unsigned char*)result, 4);
	printf("\n");
	//*************************************************//
	
	const byte test_string2[3] = { '6', 'U', 'U' };
	//const byte test_string2[3] = { 'U', 'U', '6' };
	printf("test_string2:");
	test_hex_print(test_string2, 3);
	printf("\n");
	
	int cnt2 = 0;
	char result2[256];
	//result = (char *)malloc(sizeof(byte)*256);
    memset(&result2, 0, 256);

	base62_decode(test_string2, 3, (char **)&result2, &cnt2);
	
	printf("result2:");
	test_hex_print((unsigned char*)result2, 6);
	printf("\n");
	return 0;
}

//##############################################################################
