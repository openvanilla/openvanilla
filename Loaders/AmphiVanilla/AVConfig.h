// AVConfig.h: AmphiVanilla config file manager

#ifndef __AVConfig_h
#define __AVConfig_h

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "AVDictionary.h"
#include <fstream>

#ifdef __APPLE__ 
    typedef struct timespec AVTimeStamp;
#else
    typedef time_t AVTimeStamp;
#endif

class AVConfig {
public:
    AVConfig(const string &filename) : _filename(filename)
    {
        bzero(&_timestamp, sizeof(_timestamp));
    }
    
    ~AVConfig() {}
        
    void setFilename(const string &filename)
    {
        _filename = filename;
    }
        
    bool load()
    {
        PlistParser parser;
        ifstream fs;

        fs.open(_filename.c_str(), ios_base::binary);
        if (!fs.is_open()) {
            // We fill in a default dictionary
            _parent = parser.parse(defaultDictionary().c_str());
            return false;
        }
        
        fs.seekg(0, ios_base::end);
        streampos length = fs.tellg();
        fs.seekg(0);
        
        char *buf=(char*)calloc(1, (size_t)length);
        fs.read(buf, (streamsize)length);
        fs.close();
                
        _parent = parser.parse(buf);
        free(buf);
                
        if (!isValid()) {
            // We fill in a default dictionary
            _parent = parser.parse(defaultDictionary().c_str());        
            return false;
        }
        
        _timestamp = getFileTimeStamp(_filename.c_str());
        return true;
    }
    
    bool write()
    {
        ofstream fs;
        fs.open(_filename.c_str(), ios::out | ios::binary);
        if (!fs.is_open()) return false;
        fs << _parent.str(0, xmlhead());
        fs.close();
        return true;
    }
    
    AVDictionary dictionary()
    {
        if (!isValid()) return AVDictionary();
        
        // child: #parent #plist> #dict
        return AVDictionary(_parent.childAtIndex(0)->childAtIndex(0), false);
    }
    
    bool needSync()
    {
        AVTimeStamp fts = getFileTimeStamp(_filename);
        return !isTimeStampEqual(&_timestamp, &fts);
    }

protected:
    bool isValid()
    {
        // first node: <plist>
        PlistNode *plist = _parent.childAtIndex(0);
        if (!plist) return false;
        
        // child: <dict>
        PlistNode *dict = plist->childAtIndex(0);
        if (!dict) return false;    
        
        return true;
    }

    string xmlhead()
    {
        const char *data="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
        
        return string(data);
    }
    
    string defaultDictionary()
    {
        const char *data="<plist version=\"1.0\"><dict></dict></plist>\n";
        string def = xmlhead();
        def += data;
        return def;
    }

    AVTimeStamp getFileTimeStamp(const string &filename)
    {
        const char *f=filename.c_str();
        struct stat s;
        AVTimeStamp t;
        bzero(&t, sizeof(t));
        if (f && !stat(f, &s))
        {
            #ifdef __APPLE__
                t=s.st_mtimespec;
            #else
                t=s.st_mtime;
            #endif
        }
        return t;
    }
    
    bool isTimeStampEqual(const AVTimeStamp *s1, const AVTimeStamp *s2)
    {
        return !memcmp(s1, s2, sizeof(AVTimeStamp));
    }

    string _filename;
    AVTimeStamp _timestamp;
    PlistNode _parent;
    AVDictionary *_dictionary;
};

#endif
