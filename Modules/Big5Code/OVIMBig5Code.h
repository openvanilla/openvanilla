#ifndef OVIMBig5Code_H
#define OVIMBig5Code_H

#include <stdio.h>
#include "OpenVanilla.h"

namespace OpenVanilla {
    using namespace std;

    class OVIMArrayContext;

    class OVIMBig5Code : public OVInputMethod {
    public:
        OVIMBig5Code();
        ~OVIMBig5Code();

        virtual OVEventHandlingContext* createContext();
        virtual const string identifier() const;
        virtual const string localizedName(const string& locale);
        virtual bool initialize(OVPathInfo* pathInfo, OVLoaderService* loaderService);
        virtual void loadConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);
        virtual void saveConfig(OVKeyValueMap* moduleConfig, OVLoaderService* loaderService);
    };
};


#endif
