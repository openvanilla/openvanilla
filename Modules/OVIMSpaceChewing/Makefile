# Makefile for OVIMSpaceChewing

MODULEID=OVIMSpaceChewing

SOURCES=OVIMChewingStatic Chewingpp \
	char chewingio chewingutil choice dict hanyupinying \
	hash key2pho tree userphrase zuin

DATA=chewingdata/ch_index.dat chewingdata/dict.dat chewingdata/fonetree.dat \
     chewingdata/ph_index.dat chewingdata/pinyin.tab chewingdata/us_freq.dat


INC+=-I./libchewing
VPATH+=./libchewing/

include ../Mk/ov.module.mk
