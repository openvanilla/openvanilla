IM=OVIMCangjei OVIMPOJ OVIMPhonetic OVIMXcin

all:
	for i in $(IM) ; do make -C $$i all ;done

install: all
	for i in $(IM) ; do make -C $$i install ;done

clean:
	for i in $(IM) ; do make -C $$i clean ;done


