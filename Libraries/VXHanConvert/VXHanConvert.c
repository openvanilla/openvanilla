/*
 * VXHanConvert.c Traditional/Simplified Chinese converter
 *
 * Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
 *
 * Derived from OVOFHanConvert of the OpenVanilla Project
 * Copyright (c) 2004-2008 The OpenVanilla Project (http://openvanilla.org)
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "VXHanConvert.h"
#include <stdlib.h>

extern const size_t VXHCSC2TCTableSize;
extern const size_t VXHCTC2SCTableSize;
extern const uint16_t VXHCSC2TCTable[];
extern const uint16_t VXHCTC2SCTable[];

struct VXHCData
{
    uint16_t key;
    uint16_t value;
};

static int VXHCCompare(const void *a, const void *b)
{
    uint16_t x = ((const struct VXHCData *)a)->key;
    uint16_t y = ((const struct VXHCData *)b)->key;

    if (x == y) {
        return 0;
    }

    if (x < y) {
        return -1;
    }

    return 1;
}

static uint16_t VXHCFind(uint16_t key, const uint16_t *table, size_t size)
{
    struct VXHCData k = { .key = key };
    struct VXHCData *d = (struct VXHCData *)bsearch(&k, table, size,
        sizeof(struct VXHCData), VXHCCompare);

    if (!d) {
        return 0;
    }

    return d->value;
}

uint16_t VXUCS2TradToSimpChinese(uint16_t c)
{
    return VXHCFind(c, VXHCTC2SCTable, VXHCTC2SCTableSize);
}

uint16_t VXUCS2SimpToTradChinese(uint16_t c)
{
    return VXHCFind(c, VXHCSC2TCTable, VXHCSC2TCTableSize);
}
