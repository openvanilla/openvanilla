#ifndef _DUMMYDICTIONARY_H
#define _DUMMYDICTIONARY_H

#include<map>
#include<string>
#include<sstream>
#include<cctype>

// Abstract interface for a simple dictionary. It is recommended that this
// dictionary be implemented as a type-free dictionary, i.e. you can replace
// the key with any value of any type, and type conversion between integer and
// string is done transparently, like what is done in e.g. sqlite3.
class DummyDictionary : public OVDictionary {
protected:
   std::map<std::string, std::string> _dict;
public:
    virtual int keyExist(const char *key) {
       return _dict.find(key) != _dict.end();
    }
    virtual int getInteger(const char *key) {
        return atoi(_dict[key].c_str());
    }
    virtual int setInteger(const char *key, int value) {
       std::stringstream ss;
       ss << value;
       _dict.insert( std::make_pair(key, ss.str()) );
        return value;
    }
    virtual const char* getString(const char *key) {
        return _dict[key].c_str();
    }
    virtual const char* setString(const char *key, const char *value) {
       _dict.insert( std::make_pair(key, value) );
       return value;
    }
};

#endif
