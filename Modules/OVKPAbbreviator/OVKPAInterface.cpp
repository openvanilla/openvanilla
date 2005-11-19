// OpenVanilla Library Wrappers

#include "OVKPAbbreviator.h"

SQLite3* phdb=NULL;
char* last=NULL;

extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; }

extern "C" int OVInitializeLibrary(OVService* s, const char* mp) {
    murmur("OVKPAbbreviator library initialized, module path=%s, user path=%s, path separator=%s, local=%s", mp, s->userSpacePath("OVKPAbbreviator"), s->pathSeparator(), s->locale());
    
    murmur("OVKPAbbreviator: opening database");
    char *p=sqlite3_mprintf("%s%s", s->userSpacePath("OVKPAbbreviator"), "ovkpabbreviator.db");
    phdb=new SQLite3;
    if (phdb->open(p) != SQLITE_OK) {
        murmur("OVKPAbbreviator: database open failed");
        phdb=NULL;
        return false;
    }
    
    SQLite3Statement *st=phdb->prepare("create table abbreviation(key, value, time);");
    int x;
    if (!st) {
        murmur("OVKPAbbreviator: db statement prepare failed");
    }
    else {
        if ((x = st->step()) != SQLITE_DONE) {
            murmur("OVKPAbbreviator: db create table returns %d", x);
        }
    }    
    
    return true;
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    if (idx==0) return new OVOFAbbreviationCatcher(last);
    if (idx==1) return new OVKPAbbreviator(phdb, last);
    return NULL;
}
