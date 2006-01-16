# Makefile for OpenVanilla Standard Set of Modules
#
# Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of OpenVanilla nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

MOD= OVIMArray OVIMAutoPhonetic OVIMBig5 OVIMBoshiamy OVIMChewing \
     OVIMGeneric OVIMGenericSQLite OVIMKeypadExample OVIMPOJ-Holo \
     OVIMPhonetic OVIMRoman OVIMRomanNew OVIMSpaceChewing OVIMTibetan \
     OVIMUnicode OVOFAntiZhuyinwen OVOFHanConvert \
     OVOFDisUniHex OVOFExample OVOFFullWidthCharacter OVOFHTMLCharEntity \
     OVOFQuotationmark OVOFReverseLookup OVOFSpeak

all:
	for i in $(MOD) ; do make -C $$i all ;done

install: all
	for i in $(MOD) ; do make -C $$i install ;done

uninstall:
	for i in $(MOD) ; do make -C $$i uninstall ;done

clean:
	for i in $(MOD) ; do make -C $$i clean ;done

MOD=OVIMArray OVIMGeneric OVIMPOJ-Holo OVIMPhonetic OVIMSpaceChewing \
    OVKPPhraseTools OVOFFullWidthCharacter OVOFHanConvert \
OVOFReverseLookup
OVOFSpeak
OVOFZhWikipidealize
REFACTORING.txt
SharedData
SharedHeaders
SharedSource
