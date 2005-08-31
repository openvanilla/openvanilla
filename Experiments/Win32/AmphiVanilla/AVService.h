#include "OpenVanilla.h"
#include "AVConfig.h"

class AVService : public OVService {
public:
    virtual void beep() {}
    virtual void notify(const char *msg) {}
    virtual const char *locale() { return "zh_TW"; }
    virtual const char *userSpacePath(const char *modid) { return cfg.getUserDir(); }
    virtual const char *pathSeparator();
    virtual const char *toUTF8(const char *encoding, const char *src);
    virtual const char *fromUTF8(const char *encoding, const char *src);
    /* copy from OVPhoneticLib.cpp: VPUTF16ToUTF8 */
    virtual const char *UTF16ToUTF8(unsigned short *s, int l);
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) {
        return 0;
    }
private:
    char internal[1024];
    AVConfig cfg;
};
