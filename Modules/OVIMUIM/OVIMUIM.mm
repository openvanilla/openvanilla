// OVIMUIM.cpp
// Copyright (c) 2004-2005 The OpenVanilla Project (http://openvanilla.org)

#define OV_DEBUG
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <Cocoa/Cocoa.h>
#include <UIM/uim.h>
#include <string>
#include <vector>

using namespace std;

class OUCandidate {
public:
    OUCandidate() {
        clear(); perpage=10;
    }
    
    void clear() { list.clear(); count=pos=pagefrom=0; }
    
    void add(const char *s) {
        list.push_back(string(s));
        count=list.size();
    }
    void setPerPageItems(int l) { perpage=l; }
    
    void pageUp() {
        pagefrom-=perpage;
        checkBound();
    }
    
    void pageDown() {
        pagefrom+=perpage;
        checkBound();
    }

    bool canPageDown() {
        if (pagefrom+perpage >= count) return false;
        return true;
    }

    void checkBound() {
        if (pagefrom >= count) pagefrom=count-perpage;
        if (pagefrom < 0) pagefrom=0;
    }

    void set(int p) {
        int pageto=pagefrom+perpage;          // pageto = lowerbound +1
        if (pageto >= count) pageto=count;

        if (p >= pageto) {  // next page
            pagefrom=p;
            checkBound();
        }
        if (p < pagefrom) { // previous page
            pagefrom=p-perpage+1;
            checkBound();
        }
        if (!pos && p==count-1) {    // back from 0, special case
            pagefrom=p-perpage+1;
            checkBound();                            
        }
        
        pos=p;
    }
    
    int digits(int x) {
        int d=0;
        do {
            x=x/10;
            d++;
        } while(x);
        
        return d;
    }
    
    void update(OVCandidate *c) {
        int pageto=pagefrom+perpage;          // pageto = lowerbound +1
        if (pageto >= count) pageto=count;
        
        int pagecount=(pageto-pagefrom);
        
        c->clear();
        
        // get the formatting string right
        int ptd=digits(pageto-1);
        char formatstr[256];
        // char ds[256];
        sprintf(formatstr, "%%0%dd.\t%%s", ptd);
        
/*        sprintf(ds, "%d", ptd);
        strcpy(formatstr, "%0");
        strcat(formatstr, ds);
        strcat(formatstr, "d. \t%s"); */
        
        // test run, get the longest line
        const char *d;
        char buf[256];
        size_t lgst=0;
        for (int i=pagefrom; i < pageto; i++) {
            d=list[i].c_str();
            sprintf(buf, formatstr, i, d);
            if (strlen(buf) > lgst) lgst=strlen(buf);
        }
        
        
        for (int i=pagefrom; i < pageto; i++) {
            d=list[i].c_str();
            sprintf(buf, formatstr, i, d);
            while(strlen(buf) < lgst) strcat(buf, " ");
            
            if (i==pos) strcat (buf, " «« ");
            if (i!=pageto-1 && pagecount>1) strcat(buf, "\n");
            c->append(buf);
        }
        c->update();
    }
    
    
protected:
    int count, pos, perpage;
    int pagefrom;

    vector<string> list;
};

class OUEncodingConvertor {
public:
    OUEncodingConvertor(NSStringEncoding e) { tocode=e; }
    char *convert(const char *fromstr) {
        NSString *s=[NSString stringWithCString:fromstr encoding:tocode];
        const char *u8=[s UTF8String];
        char *rs=(char*)calloc(1, strlen(u8)+1);
        strcpy(rs, u8);
        return rs;
    }
protected:
    NSStringEncoding tocode;
};

int OUEncodingIsConvertible(const char *to, const char *from) {
    return 1;
}

void *OUEncodingCreate(const char *to, const char *from) {
    return new OUEncodingConvertor(CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingEUC_JP));
}

char *OUEncodingConvert(void *obj, const char *str) {
    return ((OUEncodingConvertor*)obj)->convert(str);
}

void OUEncodingRelease(void *obj) {
    delete (OUEncodingConvertor*)obj;
}



class OVIMUIMContext : public OVInputMethodContext
{
public:
    OVIMUIMContext() {
        ovbuf=NULL;
        ovcandi=NULL;
        ovsrv=NULL;
    }
    
    void setUIMContext(uim_context c) {
        uc = c;
    }
    virtual int keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* c, OVService* srv) {
        // retain objects for the uim callbacks
        ovbuf=b;
        ovcandi=c;
        ovsrv=srv;
        
        int keycode=k->code();
        if (k->isFunctionKey()) return 0;

        // convert keycode to UIM keycode
        switch (k->code()) {
            case ovkHome:       keycode=UKey_Home; break;
            case ovkEnd:        keycode=UKey_End; break;
            case ovkPageUp:     keycode=UKey_Prior; break;
            case ovkPageDown:   keycode=UKey_Next; break;
            case ovkLeft:       keycode=UKey_Left; break;
            case ovkRight:      keycode=UKey_Right; break;
            case ovkUp:         keycode=UKey_Up; break;
            case ovkDown:       keycode=UKey_Down; break;
            case ovkReturn:     keycode=UKey_Return; break;
            case ovkEsc:        keycode=UKey_Escape; break;
            case ovkDelete:     keycode=UKey_Delete; break;
            case ovkBackspace:  keycode=UKey_Backspace; break;
        }

        int r=uim_press_key(uc, keycode, 0);
        uim_release_key(uc, keycode, 0);
        
        if (!r) return 1;
        return 0;
    }
    
// UIM callbacks
public:
    void uimClear() {
        if (!ovbuf) return;
        ovbuf->clear()->update();
        upos=uhighlightstart=uhighlightend=0;
        ucountpos=1;
    }
    
    void uimPush(int state, const char *s) {
        // UIM states
        // UPreeditAttr_UnderLine = 1,          // draw underline (NORMAL MODE)
        // UPreeditAttr_Reverse = 2,            // draw "candidate line" (HIGHLIGHT)
        // UPreeditAttr_Cursor = 4,             // cursor stops here
        // binary OR op: (2 | 4) = 6            // candidate lne & cursor stops here
        
        murmur("uimPush, state=%d, string=%d, strlen=%s", state, s ? strlen(s): 0, s ? s :0);
        
        int charcount=0;
        if (s) {
            NSString *ns=[NSString stringWithUTF8String:s];
            charcount=[ns length];
        }
        if (state & UPreeditAttr_Reverse) {
            // if (!ovcandi->onScreen()) ovsrv->notify("漢字変換モード, ←/→ to move, ↑↓ for candidates");
            uhighlightstart=upos;
            uhighlightend=upos+charcount;
        }
        if (ucountpos) upos+=charcount;
        if (state & UPreeditAttr_Cursor) ucountpos=0;      // stop counting for cursor pos

        murmur("last segment length=%d, new cursor position=%d, cursor counting=%s",
            charcount, upos, ucountpos ? "CONTINUING" : "STOPPED");

        if (s) ovbuf->append(s);
    }
    
    void uimUpdate() {
        murmur("uimUpdate: updating composing (preedit buffer)");
        if (ovbuf) ovbuf->update(upos, uhighlightstart, uhighlightend);
        upos=uhighlightstart=uhighlightend=0;
        ucountpos=1;
    }
    
    void uimCommit(const char *s) {
        murmur("uimCommit, commit string=%s", s);
        if (ovbuf) ovbuf->clear()->append(s)->send();
    }
    
    void uimCandidateActivate(int nr, int display_limit) {
        murmur("candidate activate, nr=%d, limit=%d", nr, display_limit);
        ccount=0;
        cindex=0;
        cperpage=display_limit;
        ccount=nr;
        
        ovcandi->clear();
        cdi.clear();
        cdi.setPerPageItems(display_limit);

        for (int i=0; i<nr; i++) {
            uim_candidate c;
            c=uim_get_candidate(uc, i, 0);  // the "acclerator hint" (0) doesn't seem to work
            const char *str=uim_candidate_get_cand_str(c);
            
            cdi.add(str);
            
            /* char buf[256];
            sprintf(buf, "%d.\t%s", i, str);
            ovcandi->append(buf);
            if (i != nr-1) ovcandi->append("\n"); */
            
            uim_candidate_free(c);
        }
        
        cdi.update(ovcandi);
        ovcandi->show();
        
        // ovcandi->update()->show();
        // ovsrv->notify("candidate mode,  ↑↓ to move or ESC");
    }
    
    void uimCandidateSelect(int index) {
        cindex=index;
    
        murmur("candidate select! index=%d", index);

        uim_candidate c;
        c=uim_get_candidate(uc, index, 0);  // try replace 0
        const char *str=uim_candidate_get_cand_str(c);

        char buf[256];
        sprintf(buf, "↑↓ candidate = %s (%d)", str, index);
        // ovsrv->notify(buf);

        cdi.set(index);
        cdi.update(ovcandi);

        uim_candidate_free(c);
    }
    
    void uimCandidateShiftPage(int direction) {
        // this is called when PgUp/PgDown is entered,
        // we have to do our own page flipping though
        murmur("uim candidate shift page! direction=%d", direction);
        char buf[256];
        sprintf(buf, "page shift, direction=%d", direction);
        // ovsrv->notify(buf);
        
        murmur ("set index");
        if (direction) {
            if (!cdi.canPageDown()) {
                ovsrv->beep();
            }
            else {
                cdi.pageDown();
                cindex += cperpage;
                if (cindex >= ccount) cindex=ccount-1;
            }
        }
        else {
            cindex-=cperpage;
            if (cindex < 0) {
                cindex=0;
                ovsrv->beep();
            }
            cdi.pageUp();
        }
        uim_set_candidate_index(uc, cindex);
        cdi.set(cindex);
        cdi.update(ovcandi);
        
        // uimCandidateSelect will be called, so we do nothing
    }
    
    void uimCandidateDeactivate() {
        murmur("uim candidate list deactivated");
        ovcandi->hide()->clear()->update();
    }
    
protected:
    OVBuffer *ovbuf;
    OVCandidate *ovcandi;
    OVService *ovsrv;
    uim_context uc;
    
    OUCandidate cdi;
    
    int upos;
    int ucountpos;
    int uhighlightstart;
    int uhighlightend;
    
    int ccount;
    int cindex;
    int cperpage;
};

// UIM callback functions

void OUPreeditClear(void *ptr) { if (ptr) ((OVIMUIMContext*)ptr)->uimClear(); }
void OUPreeditPush(void *ptr, int state, const char *s) { if (ptr) ((OVIMUIMContext*)ptr)->uimPush(state, s); }
void OUPreeditUpdate(void *ptr) { if (ptr) ((OVIMUIMContext*)ptr)->uimUpdate();  }
void OUPreeditCommit(void *ptr, const char *s) { if (ptr) ((OVIMUIMContext*)ptr)->uimCommit(s); }
void OUCandidateActivate(void *ptr, int nr, int display_limit) { if (ptr) ((OVIMUIMContext*)ptr)->uimCandidateActivate(nr, display_limit); }
void OUCandidateSelect(void *ptr, int index) { if (ptr) ((OVIMUIMContext*)ptr)->uimCandidateSelect(index); }
void OUCandidateShiftPage(void *ptr, int direction) { if (ptr) ((OVIMUIMContext*)ptr)->uimCandidateShiftPage(direction); }
void OUCandidateDeactivate(void *ptr) { if (ptr) ((OVIMUIMContext*)ptr)->uimCandidateDeactivate(); }

class OVIMUIM : public OVInputMethod
{
public:
    virtual const char* identifier() { return "OVIMUIM"; }
    virtual OVInputMethodContext *newContext() {
        OVIMUIMContext *ovc=new OVIMUIMContext;
        uim_context uc;

        // create UIM-anthy
        murmur("uim context create");
        uc=uim_create_context(ovc, "UTF-8", "ja", "anthy", &enccvtr, OUPreeditCommit);
        if (uc) {
            ovc->setUIMContext(uc);
        }
        else return NULL;
        
        uim_set_preedit_cb(uc, OUPreeditClear, OUPreeditPush, OUPreeditUpdate);
        uim_set_candidate_selector_cb(uc, OUCandidateActivate, OUCandidateSelect, OUCandidateShiftPage, OUCandidateDeactivate);
        // mode 1 is anthy's Hiragana mode
        uim_set_mode(uc, 1);

        return ovc;
    }
    virtual int initialize(OVDictionary *, OVService*, const char *mp) {    
        // THIS IS ACTUALLY NOT RIGHT, BUT WE PRESUME OVIMUIM::INIT WILL ONLY
        // BE CALLED ONCE

        murmur("OVUIM init");
        // setup UIM code convertor (currently only EUC_JP -> UTF8)        
        enccvtr.is_convertible=OUEncodingIsConvertible;
        enccvtr.create=OUEncodingCreate;
        enccvtr.convert=OUEncodingConvert;
        enccvtr.release=OUEncodingRelease;
        
        // setup UIM
        uim_init();
        return 1;
    }
    virtual const char* localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "日文輸入法 (UIM-anthy)";
        return "UIM (anthy) - Hiragana";
    }
    
protected:
    struct uim_code_converter enccvtr;
};

OV_SINGLE_MODULE_WRAPPER(OVIMUIM);

