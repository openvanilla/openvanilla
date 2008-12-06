// AVConfig.h: AmphiVanilla config file manager

#ifndef __AVConfig_h
#define __AVConfig_h

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include "AVDictionary.h"

using namespace std;


class AVConfig {
public:
    /* used */
    AVConfig(const string &filename);
    ~AVConfig();
    /* used */
    void setFilename(const string &filename);
    /* used */
    bool load();
    /* used */
    bool write();
    /* used */
    AVDictionary dictionary();
    /* used */
    bool needSync();

protected:
    bool isValid();
    //string xmlhead();
    //string defaultDictionary();
    //time_t getFileTimeStamp(const string &filename);
    //bool isTimeStampEqual(const time_t *s1, const time_t *s2);

    string _filename;
    //time_t _timestamp;
    AVDictionary *_dictionary;
};

#endif
