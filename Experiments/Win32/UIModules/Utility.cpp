#include "Utility.h"
#include <cstring>

wchar_t* UTF16toWCHAR(const char *str)
{
	wchar_t *r = (wchar_t*)calloc(1024, sizeof(wchar_t));
	char tmp[5];
	int out;

	int len = strlen(str);
	for(int i = 0; i*4 < len; i++)
	{
		if(i*4 < len)
			strncpy(tmp, str + i*4, 4);
		sscanf(tmp, "%x", &out);
		r[i] = out;
	}
	r[len/4] = NULL;
	return r;
}