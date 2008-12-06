#ifndef __AVDictionary_h
#define __AVDictionary_h

#include <map>
#include <OpenVanilla/OpenVanilla.h>

// readonly means the AVDictionary instance "owns" the data nodes,
// whereas read-write (readonly==true) means the data nodes are owned
// by other instances
class AVDictionary : public OVDictionary {
public:
    
    AVDictionary();
    AVDictionary(const AVDictionary &dict);
    ~AVDictionary();

    const AVDictionary& operator=(const AVDictionary &dict);

    bool dictionaryExists(const char *key);
    AVDictionary getDictionary(const char *key, bool readonly = true);
    void createDictionaryForKey(const char *key);

    virtual bool keyExist(const char *key);
    virtual int getInteger(const char *key);
    virtual int setInteger(const char *key, int value);
    virtual const char* getString(const char *key);
    virtual const char* setString(const char *key, const char *value);

protected:
    std::map<std::string, std::string> _dict;
    bool _readonly;
};


#endif
