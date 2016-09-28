#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "crypto/sha2.h"
#include "common/utils.h"

/*
See http://www.di-mgt.com.au/sha_testvectors.html
*/
int main()
{
	SHA256_CTX ctx;
	uint8_t res[SHA256_DIGEST_LENGTH];
	char strres[SHA256_DIGEST_STRING_LENGTH];
	char* vectors[] = {
		"E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855", "",
		"CA978112CA1BBDCAFAC231B39A23DC4DA786EFF8147C4E72B9807785AFEE48BB", "a",
		"BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD", "abc",
		"F7846F55CF23E14EEBEAB5B4E1550CAD5B509E3348FBC4EFA3A1413D393CB650", "message digest",
		"71C480DF93D6AE2F1EFAD1447C66C9525E316218CF51FC8D9ED832F2DAF18B73", "abcdefghijklmnopqrstuvwxyz",
		"248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		"DB4BFCBD4DA0CD85A60C3C37D3FBD8805C77F15FC6B1FDFE614EE0A7C8FDB4C0", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"F371BC4A311F2B009EEF952DD83CA80E2B60026C8E935592D0F9C308453C813E", "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
		"CDC76E5C9914FB9281A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0", NULL
	};
	char a10[] = "aaaaaaaaaa";
	int j, i = 0;

	while(vectors[i+1] != NULL)
	{
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, (uint8_t*)vectors[i+1], strlen(vectors[i+1]));
		SHA256_Final(res, &ctx);
		bytesToHex(res, SHA256_DIGEST_LENGTH, strres);
		printf("Vector %i, result = %s, expected = %s\n", i / 2, strres, vectors[i]);
		assert(strcmp(strres, vectors[i]) == 0);
		i += 2;
	}

	// Vector 8, 1 million times "a"
	SHA256_Init(&ctx);
	for(j = 0; j < 1000000 / (sizeof(a10)-1); j++)
	{
		SHA256_Update(&ctx, (uint8_t*)a10, sizeof(a10)-1);
	}
	SHA256_Final(res, &ctx);
	bytesToHex(res, SHA256_DIGEST_LENGTH, strres);
	printf("Vector %i, result = %s, expected = %s\n", i / 2, strres, vectors[i]);
	assert(strcmp(strres, vectors[i]) == 0);

	return 0;
}
