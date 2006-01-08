# Makefile for OVIMPhonetic
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

HAS_VARIANTS=sqlite


ifeq ("$(VARIANT)", "sqlite")
	MODULEID=OVIMPhoneticSQLite
    SOURCES=OVIMPhoneticSQLite OVPhoneticLib
    LDFLAGS+=-lsqlite3
    DATA=bpmf.db
    CINS=bpmf.cin punctuation.cin
    EXTRA_GOALS=bpmf.db

    include ../Mk/ov.module.mk

bpmf.db: bpmf-schema.txt ${CINS} bpmf-sqlconvert.pl
	rm -f bpmf.db
	sqlite3 bpmf.db < bpmf-schema.txt
	cat ${CINS} | perl bpmf-sqlconvert.pl | sqlite3 bpmf.db

else
    MODULEID=OVIMPhonetic
    SOURCES=OVIMPhoneticStatic OVPhoneticLib OVPhoneticData
    CINS=bpmf.cin punctuation.cin
    EXTRA_CLEAN=OVPhoneticData.c

    include ../Mk/ov.module.mk

OVPhoneticData.c: ${CINS}
	perl convert-phonedata.pl ${CINS} > OVPhoneticData.c

endif
 







