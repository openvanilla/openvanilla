// PhoneticService.cpp

#include "AutoPhonetic.h"
#include "UTF8Utility.h"

PhoneticService::PhoneticService(SQLite3 *d, OVService *s) {
    db=d;
    srv=s;
}

void PhoneticService::beep() {
    srv->beep();
}

size_t PhoneticService::codePointCount(const string &s) {
    return UTF8CodePointCount(s);
}


const CandidateList PhoneticService::fetchBPMFCandidate(const string &q) {
    CandidateList l;
    SQLite3Statement *sth=db->prepare("select chr from bpmf where bpmf=?1 order by ord;");
    if (!sth) return l;

    sth->bind_text(1, q.c_str());
    while (sth->step()==SQLITE_ROW) l.push_back(string(sth->column_text(0)));
    delete sth;
    return l;
}

size_t PhoneticService::queryBPMF(const string &q, string& first) {
    SQLite3Statement *sth=db->prepare("select chr from bpmf where bpmf=?1 order by ord;");
    if (!sth) return 0;
    sth->bind_text(1, q.c_str());
    size_t r=0;
    if (sth->step() != SQLITE_ROW) {
        first="";
    }
    else {
        first=sth->column_text(0);
        if (sth->step() == SQLITE_ROW) r=2; else r=1;
    }
    delete sth;
    return r;
}
