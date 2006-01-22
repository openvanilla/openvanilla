// ATSComponent.h: Apple Text Service Component for OpenVanilla
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

#ifndef __ATSComponent_h
#define __ATSComponent_h

#define ATSCVERSION             0x00070200  // OpenVanilla 0.7.2
#define ATSCSCRIPT              2           // smTradCinese
#define ATSCLANGUAGE            19          // langtradChinese
#define ATSCBASERESOURCEID      16896       // (15872+script_id*512) here script_id=smTradChinese
#define ATSCCOMPONENTFLAGS		0x8000+ATSCSCRIPT*0x100+ATSCLANGUAGE
// #define ATSCSCRIPT              0x7e        // smUnicodeScript
// #define ATSCLANGUAGE            0           // langEnglish
// #define ATSCBASERESOURCEID      0xa600      // Unicode component ID
// #define ATSCCOMPONENTFLAGS      0x0000fe4a  // Unicode component flags

#define ATSCPENCILMENUID        ATSCBASERESOURCEID+1
#define ATSCBUNDLENAME          "org.openvanilla.tscomponent-tc.072"
#define ATSCBNLENGTH            $"22"       // in hexadecimal
#define ATSCIMNAME              "OpenVanilla Loader 0.7.2"
#define ATSCIMNLENGTH           $"18"       // in hexadecimal
#define ATSCVENDORCODE          'opvn'

#if defined(ppc_YES)                        // PPC architecture
    #define TARGET_REZ_MAC_PPC 1
#else
    #define TARGET_REZ_MAC_PPC 0
#endif


#if defined(i386_YES)                       // x86 architecture
    #define TARGET_REZ_MAC_X86 1
#else
    #define TARGET_REZ_MAC_X86 0
#endif //

#endif
