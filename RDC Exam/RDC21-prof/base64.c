#include <stdio.h>
#include <unistd.h>

char alphabet[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789+/";

int main()
{
	int r;
	unsigned char o[3], s[4];

	while((r = read(0, o, 3)) > 0)
	{
	  s[0] = alphabet[o[0] >> 2];
	  s[1] = alphabet[((o[0] & 0x3) << 4) | (o[1] >> 4)];
	  s[2] = alphabet[((o[1] & 0xF) << 2) | (o[2] >> 6)];
	  s[3] = alphabet[o[2] & 0x3F];
	
	  if (r == 1)
	  {
		s[1] = alphabet[(o[0] & 0x3) << 4];
		s[2] = s[3] = '=';
	  }

	  if (r == 2)
	  {
		s[2] = alphabet[(o[1] & 0xF) << 2];
		s[3] = '=';
	  }

	  write(1, s, 4);
	}
}
