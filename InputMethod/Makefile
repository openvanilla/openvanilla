IM=OVIMArray OVIMCangjei OVIMPOJHolo OVIMPOJHakka OVIMPhonetic \
   OVIMRoman OVIMSocket OVIMTibetan OVIMUnicode OVIMXcin \
   OVIMezbasic OVIMphp

all:
	for i in $(IM) ; do make -C $$i all ;done

install: all
	for i in $(IM) ; do make -C $$i install ;done

clean:
	for i in $(IM) ; do make -C $$i clean ;done

