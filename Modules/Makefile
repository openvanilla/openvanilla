MOD= OVIMArray OVIMChewing OVIMGeneric OVIMKeypadExample \
     OVIMPhoneticOld OVIMRoman OVIMTibetan OVIMUIM OVOFAntiZhuyinwen \
     OVOFConvSC2TC OVOFConvTC2SC OVOFDisUniHex OVOFFullWidthCharacter \
     OVOFHTMLCharEntity OVOFMorseCode OVOFReverseLookup OVOFZhWikipidealize

all:
	for i in $(MOD) ; do make -C $$i all ;done

install: all
	for i in $(MOD) ; do make -C $$i install ;done

clean:
	for i in $(MOD) ; do make -C $$i clean ;done

