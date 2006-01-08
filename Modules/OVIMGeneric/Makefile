IMID=OVIMGeneric

SOURCES=OVCIN OVCandidateList OVFileHandler OVIMXcin\
     	OVStringToolKit XcinCinList

DATA=cj.cin dayi3.cin iroha.cin ehq.cin	zhcn_pinyin.cin zhcn_wubizixing.cin
ALLCINS=../../Data/*.cin

include ../../Mk/ov.os.mk
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk

allcin:  
	$(CP) $(ALLCINS) $(INST_PATH)${IMID}/.
