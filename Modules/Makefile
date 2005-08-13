MOD= OVIMArray OVIMAutoPhonetic OVIMBig5 OVIMBoshiamy OVIMChewing \
     OVIMGeneric OVIMGenericSQLite OVIMKeypadExample OVIMPOJ-Holo \
     OVIMPhonetic OVIMRoman OVIMRomanNew OVIMSpaceChewing OVIMTibetan \
     OVIMUnicode OVOFAntiZhuyinwen OVOFHanConvert \
     OVOFDisUniHex OVOFExample OVOFFullWidthCharacter OVOFHTMLCharEntity \
     OVOFQuotationmark OVOFReverseLookup OVOFSpeak

all:
	for i in $(MOD) ; do make -C $$i all ;done

install: all
	for i in $(MOD) ; do make -C $$i install ;done

clean:
	for i in $(MOD) ; do make -C $$i clean ;done
