#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "crypto/md5.h"

static void
bytesToHex(uint8_t b[MD5_RESULT_LEN], char *s)
{
	static const char *hex = "0123456789ABCDEF";
	int		 q, w;

	for (q = 0, w = 0; q < 16; q++)
	{
		s[w++] = hex[(b[q] >> 4) & 0x0F];
		s[w++] = hex[b[q] & 0x0F];
	}
	s[w] = '\0';
}

/*
See https://www.cosic.esat.kuleuven.be/nessie/testvectors/hash/md5/Md5-128.unverified.test-vectors
*/
int main()
{
	md5_ctxt ctx;
	uint8_t res[MD5_RESULT_LEN];
	char strres[MD5_RESULT_LEN*2 + 1];

	// Vector 0. message="" (empty string)

	MD5Init(&ctx);
	MD5Final(res, &ctx);
	bytesToHex(res, strres);
	printf("Vector 0, result = %s\n", strres);
	assert(strcmp(strres, "D41D8CD98F00B204E9800998ECF8427E") == 0);

	// Vector 1. message="a"
	MD5Init(&ctx);
	MD5Update(&ctx, (uint8_t*)"a", 1);
	MD5Final(res, &ctx);
	bytesToHex(res, strres);
	printf("Vector 1, result = %s\n", strres);
	assert(strcmp(strres, "0CC175B9C0F1B6A831C399E269772661") == 0);

	// Vector 2. message="abc"
	MD5Init(&ctx);
	MD5Update(&ctx, (uint8_t*)"abc", 3);
	MD5Final(res, &ctx);
	bytesToHex(res, strres);
	printf("Vector 2, result = %s\n", strres);
	assert(strcmp(strres, "900150983CD24FB0D6963F7D28E17F72") == 0);


	return 0;
}
