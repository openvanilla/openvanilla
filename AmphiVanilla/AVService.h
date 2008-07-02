#include "OpenVanilla.h"
#include "AVConfig.h"
#include "AVCandidate.h"  // to provide candi service to of.
#include "AVDisplayServer.h"
#include <string.h>

class AVService : public OVService {
public:
	AVService();
	AVService(const char* dir) ;
    void setDisplayServer(AVDisplayServer *svr) {dsvr = svr;}
	void setCandi(AVCandidate *c) {candi =c;}
	AVCandidate * getCandi() {return candi;}
    virtual void beep();
    virtual void notify(const char *msg);
    virtual const char *locale();// { return "zh_TW"; }
	virtual const char *userSpacePath(const char *modid);
	//{ return  returndir.clear().append(userdir).append(modid).append("/").c_str();} // cfg.getUserDir(); }
    virtual const char *pathSeparator();
    virtual const char *toUTF8(const char *encoding, const char *src);
    virtual const char *fromUTF8(const char *encoding, const char *src);
    /* copy from OVPhoneticLib.cpp: VPUTF16ToUTF8 */
    virtual const char *UTF16ToUTF8(unsigned short *s, int l);
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr);

	
	
private:
    char internal[1024];
    unsigned short u_internal[1024];
    //AVConfig cfg;
	std::string userdir;
	std::string returnuserdir;
	std::string localname;
	AVCandidate *candi;
    AVDisplayServer *dsvr;
};
