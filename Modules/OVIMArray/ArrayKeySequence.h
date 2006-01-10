// ArrayKeySequence.h: KeySequence class for Array Input Method
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

#ifndef _ARRAYKEYSEQUENCE_H
#define _ARRAYKEYSEQUENCE_H

#include <OpenVanilla/OpenVanilla.h>
#include "OVKeySequence.h"
#include "OVCIN.h"
#include <string>

class ArrayKeySequence : public OVKeySequenceSimple
{
protected:
    OVCIN* cinTable;
public:
    ArrayKeySequence(OVCIN* tab) : cinTable(tab) {}
    virtual int length() { return len; }
    virtual int add(char c){
//        if (valid(c) == 0) return 0;
        return OVKeySequenceSimple::add(c);
    }
    virtual int valid(char c){
        std::string inKey;
        inKey.push_back(c);
        if (!cinTable->isValidKey(inKey)) return 0;
        return 1;
    
    }
    virtual std::string& compose(std::string& s){
        for (int i=0; i<len; i++)
        {
            std::string inKey;
            inKey.push_back(seq[i]);
            vector<std::string> outStringVectorRef;
            if (cinTable->getCharVectorByKey(inKey, outStringVectorRef) > 0)
                s.append(outStringVectorRef[0]);
        }
        return s;
    }
    virtual char* getSeq() { return seq; }
};

#endif
