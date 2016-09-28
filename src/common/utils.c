#include <common/utils.h>

void
bytesToHex(uint8_t* b, int len, char *s)
{
	static const char *hex = "0123456789ABCDEF";
	int		 q, w;

	for (q = 0, w = 0; q < len; q++)
	{
		s[w++] = hex[(b[q] >> 4) & 0x0F];
		s[w++] = hex[b[q] & 0x0F];
	}
	s[w] = '\0';
}

