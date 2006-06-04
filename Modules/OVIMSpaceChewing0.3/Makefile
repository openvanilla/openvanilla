# Makefile for OVIMSpaceChewing

MODULEID=OVIMSpaceChewing03

SOURCES=OVIMSpaceChewing\
        char chewingio chewingutil choice \
	dict hash tree userphrase zuin hanyupinyin mod_aux\
	chewing-utf8-util key2pho

DATA=chewingdata/ch_index.dat chewingdata/dict.dat chewingdata/fonetree.dat \
     chewingdata/ph_index.dat chewingdata/pinyin.tab chewingdata/us_freq.dat

INC+=-I./libchewing -I./libchewing/common/ 
VPATH+=./libchewing/

include ../Mk/ov.module.mk

data:
	cd libchewing/tool/; make;
	svn cat http://svn.openfoundry.org/libchewingdata/utf-8/tsi.src > chewingdata/tsi.src
	cd chewingdata/; ../libchewing/tool/sort_word;
	cd chewingdata/; ../libchewing/tool/sort_dic tsi.src;
	cd chewingdata/; ../libchewing/tool/maketree;
