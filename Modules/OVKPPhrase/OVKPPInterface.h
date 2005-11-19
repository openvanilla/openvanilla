#ifndef OVKPPINTERFACE_H
#define OVKPPINTERFACE_H
// OpenVanilla Library Wrappers
#include "OVKPPhrase.h"

extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; }

extern "C" int OVInitializeLibrary(OVService* s, const char* mp) {
    murmur("OVKPPhrase library initialized, module path=%s, user path=%s, path separator=%s, local=%s", mp, s->userSpacePath("OVKPPhrase"), s->pathSeparator(), s->locale());
    
    murmur("OVKPPhrase: opening database");
    char *p=sqlite3_mprintf("%s%s", s->userSpacePath("OVKPPhrase"), "ovkpphrase.db");
    phdb=new SQLite3;
    if (phdb->open(p) != SQLITE_OK) {
        murmur("OVKPPhrase: database open failed");
        phdb=NULL;
        return false;
    }
    
    SQLite3Statement *st=phdb->prepare("create table phrase(key, value, time);");
    int x;
    if (!st) {
        murmur("OVKPPhrase: db statement prepare failed");
    }
    else {
        if ((x = st->step()) != SQLITE_DONE) {
            murmur("OVKPPhrase: db create table returns %d", x);
        }
    }    
    
    return true;
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    if (idx==0) return new OVOFPhraseCatcher;
    if (idx==1) return new OVKPPhraseTool;
    return NULL;
}

#endif