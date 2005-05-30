// UTF8Utility.h

#ifndef __UTF8Utility_h
#define __UTF8Utility_h

static inline size_t UTF8CodePointCount(const char *str) {
    size_t s=0;
    const char *p=str;
    while (*p) {
        if ((*p&0xe0)==0xc0) p++; 
        else if ((*p&0xf0)==0xe0) p+=2;
        else if ((*p&0xf8)==0xf0) p+=3;
        else if ((*p&0xfc)==0xf8) p+=4;
        else if ((*p&0xfe)==0xfc) p+=5;
        s++;
        p++;
    }
    return s;
}

static inline size_t UTF8CodePointCount(const string &s) {
    return UTF8CodePointCount(s.c_str());
}

#endif