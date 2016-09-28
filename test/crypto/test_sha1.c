#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "crypto/sha1.h"
#include "common/utils.h"

/*
See http://www.di-mgt.com.au/sha_testvectors.html
*/
int main()
{
	SHA1_CTX ctx;
	uint8_t res[SHA1_DIGEST_LENGTH];
	char strres[SHA1_DIGEST_STRING_LENGTH];
	char* vectors[] = {
		"DA39A3EE5E6B4B0D3255BFEF95601890AFD80709", "",
		"86F7E437FAA5A7FCE15D1DDCB9EAEAEA377667B8", "a",
		"A9993E364706816ABA3E25717850C26C9CD0D89D", "abc",
		"C12252CEDA8BE8994D5FA0290A47231C1D16AAE3", "message digest",
		"32D10C7B8CF96570CA04CE37F2A19D84240D3A89", "abcdefghijklmnopqrstuvwxyz",
		"84983E441C3BD26EBAAE4AA1F95129E5E54670F1", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		"761C457BF73B14D27E9E9265C46F4B4DDA11F940", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		"50ABF5706A150990A08B2C5EA40FA0E585554732", "12345678901234567890123456789012345678901234567890123456789012345678901234567890",
		"34AA973CD4C4DAA4F61EEB2BDBAD27316534016F", NULL
	};
	char a10[] = "aaaaaaaaaa";
	int j, i = 0;

	while(vectors[i+1] != NULL)
	{
		SHA1_Init(&ctx);
		SHA1_Update(&ctx, (uint8_t*)vectors[i+1], strlen(vectors[i+1]));
		SHA1_Final(res, &ctx);
		bytesToHex(res, SHA1_DIGEST_LENGTH, strres);
		printf("Vector %i, result = %s, expected = %s\n", i / 2, strres, vectors[i]);
		assert(strcmp(strres, vectors[i]) == 0);
		i += 2;
	}

	// Vector 8, 1 million times "a"
	SHA1_Init(&ctx);
	for(j = 0; j < 1000000 / (sizeof(a10)-1); j++)
	{
		SHA1_Update(&ctx, (uint8_t*)a10, sizeof(a10)-1);
	}
	SHA1_Final(res, &ctx);
	bytesToHex(res, SHA1_DIGEST_LENGTH, strres);
	printf("Vector %i, result = %s, expected = %s\n", i / 2, strres, vectors[i]);
	assert(strcmp(strres, vectors[i]) == 0);

	return 0;
}
