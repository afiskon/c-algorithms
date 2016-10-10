#include <common/utils.h>
#include <string.h>


inline static int
hexToInt(char c)
{
	if(c >= '0' && c <= '9')
		return (c - '0');

	if(c >= 'A' && c <= 'F')
		return (10 + c - 'A');

	if(c >= 'a' && c <= 'f')
		return (10 + c - 'a');

	return -1;
}

int
hexToBytes(char* s, uint8_t* b, int blen)
{
	int i, th, tl, slen = strlen(s);
	if(slen % 2 != 0)
		return -1;
	if(slen / 2 != blen)
		return -2;

	for(i = 0; i < blen; i++)
	{
		th = hexToInt(s[i*2]);
		tl = hexToInt(s[i*2+1]);
		if(th < 0 || tl < 0)
			return -1;

		b[i] = ((uint8_t)th << 4) | (uint8_t)tl;
	}

	return 0;
}

void
bytesToHex(uint8_t* b, int blen, char *s)
{
	static const char *hex = "0123456789ABCDEF";
	int		 q, w;

	for (q = 0, w = 0; q < blen; q++)
	{
		s[w++] = hex[(b[q] >> 4) & 0x0F];
		s[w++] = hex[b[q] & 0x0F];
	}
	s[w] = '\0';
}

