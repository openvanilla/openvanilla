// PhoneticSyllable.cpp

#include "AutoPhonetic.h"


PhoneticSyllable::PhoneticSyllable(PhoneticService *s, const PhoneticConfig &c)
{
    cfg=c;
    bpmf.setLayout(c.layout);
    srv=s;
    reset();
}

void PhoneticSyllable::updateConfig(const PhoneticConfig &c) {
    cfg=c;
    bpmf.setLayout(c.layout);
}

void PhoneticSyllable::reset() {
    bpmf.clear();
    fixedusrchr=false;
    hascandi=false;
    committed=false;
    usedb=true;
    cur=-1;
    seq="";
    chr="";
}

bool PhoneticSyllable::hasCandidate() {
    return hascandi;
}

bool PhoneticSyllable::fixedUserChar() {
    return fixedusrchr;
}

void PhoneticSyllable::changeChar(const string& c, bool fixusrchr) {
    if (!c.length()) return;
    chr=c;
    fixedusrchr=fixusrchr;
}


const PhoneticConfig& PhoneticSyllable::getConfig() {
    return cfg;
}

const string PhoneticSyllable::bpmfString() {
    const string &codestr=code();
    int l=codestr.length();
    if (!l) return string();
    char lc=codestr.c_str()[l-1];
    if (lc=='3' || lc=='4' || lc=='6' || lc=='7') l--;
    return codestr.substr(0, l);
}

const string PhoneticSyllable::toneString() {
    const string &codestr=code();
    int l=codestr.length();
    if (!l) return string();
    char lc=codestr.c_str()[l-1];
    switch (lc) {
        case '3': return string("3");
        case '4': return string("4");
        case '6': return string("2");
        case '7': return string("5");
    }
    return string("0");
}

const string& PhoneticSyllable::code() {
    if (!committed) seq=bpmf.standardLayoutCode();
    return seq;
}

const string& PhoneticSyllable::presentation() {
    if (!committed) chr=bpmf.compose();     // compose BPMF syllable
    return chr;
}

bool PhoneticSyllable::vacant() {
    // if committed and either is empty, then it is vacant
    if (committed) {
        if (!chr.length() || !seq.length()) return true; else return false;
    }
    
    if (bpmf.empty()) return true;
    return false;
}

size_t PhoneticSyllable::width() {
    return UTF8CodePointCount(presentation());
}

size_t PhoneticSyllable::cursor() {
    if (cur<0 || cur>=(int)width()) {
        cur=-1;
        return width();
    }
    return cur;
}

const TEvent PhoneticSyllable::keyEvent(KeyCode k, KeyModifier m) {
    if (committed) return TEvent();     // if committed, no more key response

    // if it's CTRL-0/CTRL-1/CTRL-OPT-[a-z], do punctuation
    if (isPunctuationCombination(k, m) && vacant()) return keyPunctuation(k, m);
    
    // send back all function key (CTRL/OPT/CMD) combinations
    if ((m & K_FUNCTIONKEYMASK) && vacant()) return TEvent();

    // caps lock processing
    if ((m & K_CAPSLOCK) && vacant()) return keyCapslock(k, m);
    
    if (k==K_ESC) return keyEsc();
    if (k==K_LEFT) return keyLeft();
    if (k==K_RIGHT) return keyRight();
    if (k==K_BACKSPACE) return keyBackspace();
    if (k==K_DELETE) return keyDelete();
    if (!vacant() && (bpmf.isComposeKey(k)||k==K_RETURN)) return keyCompose(k);
    if (isprint(k)) return keyPrintable(k, m);
    if (!vacant()) {
        srv->beep();
        return TEvent(T_UPDATE);
    }
    return TEvent();
}

const TEvent PhoneticSyllable::cancelCandidate() {
    reset();
    return TEvent(T_CLEAR);
}

const TEvent PhoneticSyllable::chooseCandidate(size_t, const string &item) {
    committed=true;
    chr=item;
    return TEvent(T_COMMIT);
}

const string PhoneticSyllable::dump() {
    char d[256];
    sprintf(d, "syllable fixedusrchr=%s, hascandi=%s, committed=%s, usedb=%s, bpmf='%s', seq='%s', chr='%s'",
        fixedusrchr ? "TRUE" : "FALSE", hascandi ? "TRUE" : "FALSE",
        committed ? "TRUE" : "FALSE", usedb ? "TRUE" : "FALSE",
        bpmf.standardLayoutCode(), seq.c_str(), chr.c_str());
    return string(d);
}

const CandidateList PhoneticSyllable::fetchCandidateList() {
    if (!usedb) return CandidateList();
    return srv->fetchBPMFCandidate(*this);    
}

bool PhoneticSyllable::isPunctuationCombination(KeyCode k, KeyModifier m) {
    // only accept CTRL-1 or CTRL-0, or only accept CTRL-OPT-[printable]
    if (((m&K_CTRL) && !(m&K_OPT) && !(m&K_COMMAND) && (k=='1' || k=='0')) ||
        ((m&K_CTRL) && (m&K_OPT) && !(m&K_COMMAND) && !(m&K_SHIFT) &&
         ((k>=1 && k<=26) || isprint(k)))) return true;
    return false;
}

const TEvent PhoneticSyllable::keyPunctuation(KeyCode k, KeyModifier m){
    size_t c;
    string first;
    string newseq;
    if ((k=='0' || k=='1') && !(m&K_OPT))
        newseq="_punctuation_list";
    else {
        newseq=string("_ctrl_opt_")+string(1, ((k>=1 && k <=26) ? k+'a'-1 : k));
    }

    cur=-1;
    c=srv->queryBPMF(newseq, first);
    
    if (!c) return TEvent();
    chr=first;
    committed=true;
    if (c==1) return TEvent(T_COMMIT);    
    
    // has more than one candidate, hascandi set to true
    hascandi=true;
    return TEvent(T_CANDIDATE);
}

const TEvent PhoneticSyllable::keyCapslock(KeyCode k, KeyModifier m){
    cur=-1;
    if (!isprint(k)) return TEvent();
    committed=true;
    usedb=false;
    chr=(m & K_SHIFT) ? string(1, toupper(k)) : string(1, tolower(k));
    seq=chr;
    return TEvent(T_COMMIT);
}

const TEvent PhoneticSyllable::keyEsc(){
    cur=-1;
    if (vacant() || committed) return TEvent();
    reset();
    return TEvent(T_CLEAR);
}

const TEvent PhoneticSyllable::keyLeft(){
    if (vacant()) return TEvent();
    if (cur==0) {
        srv->beep();
    }
    else if (cur==-1) {
        cur=width()-1;
    }
    else cur--;
    return TEvent(T_UPDATE);
}

const TEvent PhoneticSyllable::keyRight(){
    if (vacant()) return TEvent();
    if (cur <0 || cur>=(int)width()) {
        srv->beep();
    }
    else cur++;
    return TEvent(T_UPDATE);
}

const TEvent PhoneticSyllable::keyBackspace(){
    // backspace handling, we delete a character from the standard layout code
    if (vacant()) return TEvent();
    if (!cur) {
        srv->beep();
        return TEvent(T_UPDATE);
    }
    if (cur==(int)width() || cur==-1) {
        cur=-1;
        bpmf.removeLast();
    }
    else {
        char buf[16];
        strcpy(buf, bpmf.standardLayoutCode());
        int i, l=strlen(buf);
        for (i=cur-1; i<l-1; i++) buf[i]=buf[i+1];
        buf[--l]=0;
        bpmf.clear();
        bpmf.setLayout(OVPStandardLayout);
        for (i=0; i<l; i++) bpmf.addKey(buf[i]);
        bpmf.setLayout(cfg.layout);
        cur--;
    }
        
    if (bpmf.empty()) {
        reset();
        return TEvent(T_CLEAR);
    }
    return TEvent(T_UPDATE);
}

const TEvent PhoneticSyllable::keyDelete(){
    if (vacant()) return TEvent();
    if (cur==(int)width() || cur==-1) {
        srv->beep();
        return TEvent(T_UPDATE);
    }

    char buf[16];
    strcpy(buf, bpmf.standardLayoutCode());
    int i, l=strlen(buf);
    for (i=cur; i<l-1; i++) buf[i]=buf[i+1];
    buf[--l]=0;
    bpmf.clear();
    bpmf.setLayout(OVPStandardLayout);
    for (i=0; i<l; i++) bpmf.addKey(buf[i]); 
    bpmf.setLayout(cfg.layout);
    if (bpmf.empty()) {
        reset();
        return TEvent(T_CLEAR);
    }
    return TEvent(T_UPDATE);
}

const TEvent PhoneticSyllable::keyCompose(KeyCode k){
    if (k!=K_SPACE && k!=K_RETURN) bpmf.addKey(k);

    string first;
    string newseq=string(bpmf.standardLayoutCode());
    size_t c=srv->queryBPMF(*this, first);
        
    cur=-1;
    if (!c) {
        srv->beep();
        return TEvent(T_UPDATE);
    }

    chr=first;
    seq=newseq;
    committed=true;
    if (c==1) return TEvent(T_COMMIT);
    
    hascandi=true;
    return TEvent(T_CANDIDATE);
}

const TEvent PhoneticSyllable::keyPrintable(KeyCode k, KeyModifier m) {
    cur=-1;
    if (isalpha(k) && (m&K_SHIFT) && vacant()) {
        committed=true;
        usedb=false;
        seq=chr=string(1, k);
        return TEvent(T_COMMIT);
    }
    if (!bpmf.addKey(k)) {
        if (vacant()) return keyNonBPMF(k);
        srv->beep();
    }
    return TEvent(T_UPDATE);
}

const TEvent PhoneticSyllable::keyNonBPMF(KeyCode k){
    string first;
    string newseq=string("_punctuation_")+string(1, k);
    size_t c=srv->queryBPMF(newseq, first);

    cur=-1;
    if (!c) {          // not a punctuation mark
        committed=true;
        usedb=false;
        seq=chr=string(1, k);
        return TEvent(T_COMMIT);
    }
    
    committed=true;
    seq=newseq;
    chr=first;
    if (c==1) return TEvent(T_COMMIT);
    
    hascandi=true;
    return TEvent(T_CANDIDATE);
}
