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
	char* vectors[] = {
		"D41D8CD98F00B204E9800998ECF8427E", "",
		"0CC175B9C0F1B6A831C399E269772661", "a",
		"900150983CD24FB0D6963F7D28E17F72", "abc",
		"F96B697D7CB7938D525A2F31AAF161D0", "message digest",
		"C3FCD3D76192E4007DFB496CCA67E13B", "abcdefghijklmnopqrstuvwxyz",
		"8215EF0796A20BCAAAE116D3876C664A", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		"D174AB98D277D9F5A5611C2C9F419D9F", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"57EDF4A22BE3C955AC49DA2E2107B67A", "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
		NULL
	};
	int i = 0;

	while(vectors[i] != NULL)
	{
		MD5Init(&ctx);
		MD5Update(&ctx, (uint8_t*)vectors[i+1], strlen(vectors[i+1]));
		MD5Final(res, &ctx);
		bytesToHex(res, strres);
		printf("Vector %i, result = %s, expected = %s\n", i / 2, strres, vectors[i]);
		assert(strcmp(strres, vectors[i]) == 0);
		i += 2;
	}

	return 0;
}
