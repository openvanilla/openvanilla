For OVIMPhoneticStatic.vcproj, use

	perl convert-phonedata.pl ${CINS} > OVPhoneticData.c

to generate OVPhoneticData.c first, where ${CINS} can be

	bpmf.cin punctuations.cin
or
	bpmf-ext.cin punctuations.cin