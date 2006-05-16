# Makefile for OVIMSpaceChewing

MODULEID=OVIMSpaceChewing0.3

SOURCES=OVIMSpaceChewing\
	char chewingio chewingutil choice dict hanyupinying \
	hash tree userphrase zuin

DATA=chewingdata/ch_index.dat chewingdata/dict.dat chewingdata/fonetree.dat \
     chewingdata/ph_index.dat chewingdata/pinyin.tab chewingdata/us_freq.dat


INC+=-I./libchewing
INC+=-I./libchewing/porting_layer/include
INC+=-I./libchewing/porting_layer/include/sys
VPATH+=./libchewing/
LIBS+=./libchewing/common/libcommon.la
LIBS+=./libchewing/porting_layer/src/libporting_layer.la

include ../Mk/ov.module.mk
