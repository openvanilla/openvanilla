// VXHanConvert.cpp

#include <stdio.h>
#include <stdlib.h>
#include "VXHanConvert.h"

const size_t vxTC2SCTableSize=3214;
// const size_t vxTC2SCTableSize=3214;
extern unsigned short vxTC2SCTable[];
// extern unsigned short vxSC2TCTable[];

struct VXHCData
{
	unsigned short key, value;
};

int VXHCCompare(const void *a, const void *b)
{
	unsigned short x=((const VXHCData*)a)->key, y=((const VXHCData*)b)->key;
	if (x==y) return 0;
	if (x<y) return -1;
	return 1;
}

unsigned short VXHCFind(unsigned key, unsigned short *table, size_t size)
{
	VXHCData k;
	k.key=key;
	VXHCData *d=(VXHCData*)bsearch(&k, table, size, sizeof(VXHCData), VXHCCompare);
	if (!d) return 0;
	return d->value;
}

unsigned short VXUCS2TradToSimpChinese(unsigned short c)
{
	return VXHCFind(c, vxTC2SCTable, vxTC2SCTableSize);
}
