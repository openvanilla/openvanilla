#ifndef OVLoaderServiceImpl_h
#define OVLoaderServiceImpl_h

#include "OVLoaderService.h"

namespace OpenVanilla {
    class OVLoaderServiceImpl : public OVLoaderService {
    public:
        virtual void beep();
        virtual void notify(const string& message) {}
        virtual void HTMLNotify(const string& content) {}

        virtual const string locale() const;
        virtual const OVKey makeOVKey(int characterCode, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false);
        virtual const OVKey makeOVKey(const string& receivedString, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false);

        virtual ostream& logger(const string& sectionName = "");
    
        virtual OVDatabaseService* defaultDatabaseService();
        virtual OVDatabaseService* CINDatabaseService();
        virtual OVDatabaseService* SQLiteDatabaseService() { return 0; }
    
        virtual OVEncodingService* encodingService() { return 0; }
    
    	virtual void __reserved1(const string&) {}
        virtual void __reserved2(const string&) {}
        virtual void __reserved3(const string&) {}
    	virtual void __reserved4(const string&) {}
        virtual const string __reserved5() const { return ""; }
    	virtual void __reserved6(const string&) {}
    	virtual void __reserved7(const string&, const string &) {}
        virtual void* __reserved8(const string&) { return 0; }
    };
};

#endif
