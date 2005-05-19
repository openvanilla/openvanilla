// OVDeprecated.h
// OpenVanilla 0.6.3 -> 0.7.0 adaptor

#ifndef __OVDeprecated_h
#define __OVDeprecated_h

typedef OVCandidate OVTextBar;

typedef int OVEncoding;
typedef int OVLanguage;
typedef int OVException;

enum
{
    ovEncodingUTF8=0,
    ovEncodingUTF16Auto=1,	// use this if you have e.g. short x=0xae10;
    ovEncodingUTF16BE=2,     
    ovEncodingUTF16LE=4,
    ovEncodingBig5=16,
    ovEncodingBig5HKSCS=16,
    ovEncodingEUC_CN=24,
    ovEncodingNon8BitEncodingMask=7
};

enum
{
    ovLangAll=0,
    ovLangTradChinese=1,
    ovLangSimpChinese=2,
    ovLangJapanese=3,
    ovLangKorean=4,
	ovLangWestern=255,
};

enum
{
    ovException=0,
    ovExceptionMemory=1,
    ovExceptionBuffer=2,
    ovExceptionNotSupported=3
};

#endif
