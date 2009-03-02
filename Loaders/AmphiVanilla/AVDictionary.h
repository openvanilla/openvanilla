// AVDictionary.h: AmphiVanilla Implementaiton of OVDictionary with Plist

#ifndef __AVDictionary_h
#define __AVDictionary_h

#include "AVPlist.h"
#include <OpenVanilla/OpenVanilla.h>

// readonly means the AVDictionary instance "owns" the data nodes,
// whereas read-write (readonly==true) means the data nodes are owned
// by other instances
class AVDictionary : public OVDictionary {
public:
    AVDictionary(PlistNode *datanode=NULL, bool readonly = true)
    {
        if (!datanode) {
            _datanode = new PlistNode("dict");
        }
        else {
            _datanode = datanode;
        }
        
        _readonly = readonly;   
    }
    
    ~AVDictionary()
    {
        if (_readonly) delete _datanode;
    }
    
    AVDictionary(const AVDictionary &dict)
    {
        _readonly = dict._readonly;
        _datanode = _readonly ? new PlistNode(*dict._datanode) : dict._datanode;
    }
    
    const AVDictionary& operator=(const AVDictionary &dict)
    {
        _readonly = dict._readonly;
        _datanode = _readonly ? new PlistNode(*dict._datanode) : dict._datanode;
        return *this;
    }
    
    bool dictionaryExists(const char *key)
    {
        PlistNode *n = _datanode->findChildByKey(key);
        if (!n) return false;
        
        if (n->name()=="dict") return true;
    }
    
    AVDictionary getDictionary(const char *key, bool readonly = true)
    {
        if (!dictionaryExists(key)) return AVDictionary();
        
        PlistNode *n = _datanode->findChildByKey(key);
        if (_readonly) return AVDictionary(new PlistNode(*n));
        return AVDictionary(n, false);        
    }
    
    void createDictionaryForKey(const char *key)
    {
        if (dictionaryExists(key)) return;
        
        PlistNode n("dict");
        _datanode->setChildByKey(key, n);
    }

    virtual bool keyExist(const char *key)
    {
        return _datanode->findChildByKey(key) ? 1 : 0;
    }
    
    virtual int getInteger(const char *key)
    {
        PlistNode *n = _datanode->findChildByKey(key);
        if (!n) return 0;
        return atoi(n->text().c_str());
    }
    
    virtual int setInteger(const char *key, int value)
    {
        stringstream s;
        s << value;
        
        PlistNode n("string", _datanode);
        n.setText(s.str());
        _datanode->setChildByKey(key, n);
        return value;
    }
    
    virtual const char* getString(const char *key)
    {
        PlistNode *n = _datanode->findChildByKey(key);
        if (!n) return "";
        return n->text().c_str();
    }
    
    virtual const char* setString(const char *key, const char *value)
    {
        PlistNode n("string", _datanode);
        n.setText(string(value));
        _datanode->setChildByKey(key, n);
        return value;
    }
    
    PlistNode *dataNode() {
        return _datanode;
    }

protected:
    PlistNode *_datanode;
    bool _readonly;
};


#endif
