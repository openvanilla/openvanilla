#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <map>
#include <string>

using namespace std;

static char *entityTable[] = {
    "&", "&amp;",
    ">", "&gt;",
    "<", "&lt;",
    "\"", "&quot;",
    "'", "&apos;",
    NULL
};

class OVOFHTMLCharEntity : public OVOutputFilter {
private:
    typedef map<string, const char*> EntityMap;
    EntityMap entityMap;
    string buf;
public:
    OVOFHTMLCharEntity() {}
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        for(int i=0; entityTable[i] ; i+=2){
            entityMap.insert( make_pair( entityTable[i], entityTable[i+1]) );
        }
        return 1;
    }

    
    virtual const char *identifier() { return "OVOFHTMLCharEntity"; }

    virtual const char *process (const char *src, OVService *srv) {
        int len = strlen(src);
        buf.clear();
        for(int i=0;i<len;i++){
            EntityMap::const_iterator it;
            string ch;
            ch.push_back(src[i]);
            if( (it = entityMap.find(ch)) != entityMap.end() )
                buf.append(it->second) ;
            else
                buf.append(ch);
        }
        return buf.c_str();
    }
    
};

 OV_SINGLE_MODULE_WRAPPER(OVOFHTMLCharEntity);
