// OVEncoding.h
//
// OpenVanilla encoding utility. This must be implemented 

#ifndef __OVEncoding_h
#define __OVEncoding_h

typedef int OVEncoding;

enum {
    oveUTF8=0,
    oveUTF16Auto=0x0001,
    oveUTF16BE=0x0002,
    oveUTF16LE=0x0004,
    oveBig5HKSCS=0x0101,
    oveEUC_CN=0x102
};


char *OVConvertToUTF8(void *src, char *dest, OVEncoding enc, int length=0);
void *OVConvertFromUTF8(const char *src, void *dest, OVEncoding enc, int maxlen=0);

#endif
