// POJ-Holo.h: Definition of POJ-Holo's key sequence
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __POJHolo_h
#define __POJHolo_h

// keyboard layouts
enum {
    POJ_Holo_ToneByNumber=0,
    POJ_Holo_ToneByPreceedingSymbol=1,
    POJ_Holo_ToneBySucceedingSymbol=2
};

// OU encoding
enum {
    OU_EncodedBy_CDRA=true
};

const int POJ_Holo_MaxSeqLen=16, POJ_Holo_MaxBufLen=64;

class POJHoloKeySequence
{
public:
    POJHoloKeySequence();
    
    bool isEmpty();
    void clear();

    bool isComposeKey(char c);
    bool add(char c, int layout=POJ_Holo_ToneByNumber);
    void remove();

    void normalize();           // normalize before finalize
    const char *sequence();
    const char *finalize();     // return finalized sequence (+ tone)
    const char *compose(bool pureascii, bool useDotComposing);

    int toneMark(char c);
    
protected:
    int len;
    char seq[POJ_Holo_MaxSeqLen];
    char composebuf[POJ_Holo_MaxBufLen];

    int presettone;
    int prebindwait;
    int bindvowel;
    
    // service functions
    int vowelorder(char c);
    const char *vowel2tone(char c, int tone, bool ou_encoding);
};

#endif
