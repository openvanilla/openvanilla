IMID=OVIMPhonetic
SOURCES=OVIMPhoneticStatic OVPhoneticLib OVPhoneticData

CINS=bpmf.cin punctuation.cin

include ../../Mk/ov.os.mk
include ../../Mk/ov.im.mk
include ../../Mk/ov.vars.mk
include ../../Mk/ov.general.mk

OVPhoneticData.c: ${CINS}
	perl convert-phonedata.pl ${CINS} > OVPhoneticData.c
