#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "crypto/rijndael.h"
#include "common/utils.h"


/*
See:

https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/rijndael/Rijndael-128-128.unverified.test-vectors
https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/rijndael/Rijndael-192-128.unverified.test-vectors
https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/rijndael/Rijndael-256-128.unverified.test-vectors
*/
int main()
{
	rijndael_ctx ctx;
	char* vectors[] = {
		// key, plain, encrypted
		"000102030405060708090A0B0C0D0E0F",
		"762A5AB50929189CEFDB99434790AAD8",
		"00112233445566778899AABBCCDDEEFF",

		"2BD6459F82C5B300952C49104881FF48",
		"E99388EED41AD8058D6162B0CF4667E6",
		"EA024714AD5C4D84EA024714AD5C4D84",

		"000102030405060708090A0B0C0D0E0F1011121314151617",
		"3369EB82973635E9C2E96D687724C790",
		"00112233445566778899AABBCCDDEEFF",

		"2BD6459F82C5B300952C49104881FF482BD6459F82C5B300",
		"A4C5C1F3858A5A596189E928B4469EA2",
		"EA024714AD5C4D84EA024714AD5C4D84",

		"000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F",
		"EAB487E68EC92DB4AC288A24757B0262",
		"00112233445566778899AABBCCDDEEFF",

		"2BD6459F82C5B300952C49104881FF482BD6459F82C5B300952C49104881FF48",
		"DFC295E9D04A30DB25940E4FCC64516F",
		"EA024714AD5C4D84EA024714AD5C4D84",
 	};
	int t, ntests = sizeof(vectors) / sizeof(vectors[0]) / 3;


	for(t = 0; t < ntests; t++)
	{
		uint8_t bkey[32]; // at most 256 bits
		uint8_t bplain[16];
		uint8_t bencrypted[16];
		char encrypted_hex[33];
		char plain_hex[33];
		char* skey = vectors[t*3];
		char* splain = vectors[t*3 + 1];
		char* sencrypted = vectors[t*3 + 2];
		int bkeylen = strlen(skey) / 2;
		int tmp;

		tmp = hexToBytes(skey, bkey, bkeylen);
		assert(tmp == 0);

		tmp = hexToBytes(splain, bplain, sizeof(bplain));
		assert(tmp == 0);

		rijndael_set_key(&ctx, (u4byte*)&bkey, bkeylen*8, 0);

		// test encryption

		rijndael_encrypt(&ctx, (u4byte*)&bplain, (u4byte*)&bencrypted);
		bytesToHex(bencrypted, sizeof(bencrypted), encrypted_hex);

		// test decryption

		memset(bplain, 0, sizeof(bplain));
		rijndael_decrypt(&ctx, (u4byte*)&bencrypted, (u4byte*)&bplain);
		bytesToHex(bplain, sizeof(bplain), plain_hex);

		printf("Test vector %d, skey = %s, splain = %s, sencrypted = %s, encrypted_hex = %s, plain_hex = %s\n",
				t, skey, splain, sencrypted, encrypted_hex, plain_hex);

		assert(strcmp(sencrypted, encrypted_hex) == 0);
		assert(strcmp(splain, plain_hex) == 0);
	}

	// TODO: cover ECB and CBC encryption

	return 0;
}
