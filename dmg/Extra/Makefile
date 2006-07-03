# Makefile for Extra Pack
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

ifeq ("$(DATASOURCE)", "")
$(error Must have DATASOURCE)
endif

ifeq ("$(MODULESOURCE)", "")
$(error Must have MODULESOURCE)
endif

ifeq ("$(DSTROOT)", "")
$(error Must have DSTROOT)
endif

all:
	mkdir -p $(DSTROOT)
	./copycin.pl $(DSTROOT) $(DATASOURCE) cj-ext.cin "輸入法資料表─倉頡（大字集）" 
	./copycin.pl $(DSTROOT) $(DATASOURCE) cj-j.cin "輸入法資料表─倉頡-J"
	./copycin.pl $(DSTROOT) $(DATASOURCE) cj5.cin "輸入法資料表─倉頡五代"
	./copycin.pl $(DSTROOT) $(DATASOURCE) corner.cin "輸入法資料表─四角號碼"
	./copycin.pl $(DSTROOT) $(DATASOURCE) jyutping.cin "輸入法資料表─粵語拼音"
	./copycin.pl $(DSTROOT) $(DATASOURCE) qcj.cin "輸入法資料表─順序速成" 
	./copycin.pl $(DSTROOT) $(DATASOURCE) scj7.cin "輸入法資料表─快倉七代" 
	./copycin.pl $(DSTROOT) $(DATASOURCE) shuangpin.cin "输入法资料表─简体双拼资料表"
	./copycin.pl $(DSTROOT) $(DATASOURCE) simplex-ext.cin "輸入法資料表─簡易（大字集）"
	./copycin.pl $(DSTROOT) $(DATASOURCE) telecode.cin "輸入法資料表─電報碼"
	./copycin.pl $(DSTROOT) $(DATASOURCE) wu.cin "輸入法資料表─吳語（全字集）"
	./copycin.pl $(DSTROOT) $(DATASOURCE) wus.cin "输入法资料表─吳语（简體）"
	./copycin.pl $(DSTROOT) $(DATASOURCE) wut.cin "輸入法資料表─吳語（繁體）" 

	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVIMBig5 "(Module) Big5 Internal Code IME" "模組─Big5內碼輸入法" "模块─繁体Big5内码输入法"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVIMPhoneticSQLite "(Module) Phonetic (Bopomofo) with Large Char. Set" "模組─注音（大字集）" "模块─注音（大字集）"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVIMRoman "(Module) Roman (Alphanumeric)" "模組─英數" "模块─英数"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVIMTibetan "(Module) Tibetan" "模組─藏文" "模块─藏文"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVIMUIM "(Module) UIM-Anthy" "模組─UIM-Anthy 日文片假名" "模块─UIM-Anthy 日文片假名"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVIMUnicode "(Module) Unicode IME" "模組─Unicode內碼輸入法" "模块─Unicode内码输入法"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVOFAntiZhuyinwen "(Module) Zhuyinwen Filter" "模組─注音文防治濾嘴" "模块─繁体注音文防治滤嘴"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVOFSmartQuotations "(Module) Smart Quotations for English, French and German" "模組─英法德智慧引號" "模块─英法德智能引号"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVOFTMLCharEntity "(Module) HTML Character Entities Filter" "模組─自動輸出HTML Entities" "模块─自动输出HTML Entities"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVOFUnicodeDecoder "(Module) Unicode Decoder" "模組─顯示輸出字之Unicode內碼" "模块─显示输出字之Unicode内码"
	./copymod.pl $(DSTROOT)  $(MODULESOURCE) OVOFZhWikipidealize "(Module) zh.Wikipedia Helper" "模組─將「．」符號轉換為中文版Wikipedia格式" "模块─将「．」符号转换为中文版Wikipedia格式"
