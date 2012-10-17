/*
 * VXHanConvert.h: Traditional/Simplified Chinese converter
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

#ifndef VXHanConvert_h
#define VXHanConvert_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t VXUCS2TradToSimpChinese(uint16_t c);
uint16_t VXUCS2SimpToTradChinese(uint16_t c);

#ifdef __cplusplus
};
#endif

#endif
