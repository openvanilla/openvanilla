// OVIMChewing.cpp

#define OV_DEBUG

#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
// #include <ChewingPP/Chewingpp.h>         // lukhnos
#include "Chewingpp.h"                      // lukhnos

char *layouts_zhtw[8] = {"標準排列","許氏","IBM","精業","倚天","倚天 26 鍵", "Dvorak","Dvorak 許氏"};
char *layouts_zhcn[8] = {"标准排列","许氏","IBM","精业","倚天","倚天 26 键", "Dvorak","Dvorak 许氏"};
char *layouts_en[8] = {"Standard","Hsu","IBM","Gin-Yeh","Eten","Eten 26", "Dvorak","Dvorak Hsu"};

int ChewingFileExist(const char *path, const char *file) {
    char buf[PATH_MAX];
    sprintf(buf, "%s/%s", path, file);
    struct stat st;
    if (stat(buf, &st)) return 0;   // doesn't exist
    return 1;
}

int ChewingCheckData(const char *path) {
    char *files[5]={"ch_index.dat", "dict.dat", "fonetree.dat", "ph_index.dat",  "us_freq.dat"};
    for (int i=0; i<5; i++) if (!ChewingFileExist(path, files[i])) return 0;
    return 1;
}

class OVIMChewing;

class OVIMChewingContext : public OVInputMethodContext 
{
public:
    OVIMChewingContext(OVIMChewing *p, Chewing* chew) {
        p=parent;
        im=chew;
    }
	
	virtual void start(OVBuffer *key, OVCandidate *textbar, OVService *srv) {	
	} 
    virtual void clear() { im->Enter(); }
    virtual void end() { im->Enter(); }
     
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
        if(key->isCommand()) return 0;
        KeyPress(key,buf,textbar,srv);
        if(im->KeystrokeIgnore()) return 0;
        CandidateWindow(textbar, srv);
        Redraw(buf, srv);
        return 1;
    }
  
protected:
    void KeyPress(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
        int k = key->code();
        Capslock(key,buf,textbar,srv);
        if(k == ovkSpace) {
            key->isShift() ? im->ShiftSpace() : im->Space();
        }
        else if (k == ovkLeft)   {
            key->isShift() ? im->ShiftLeft()  : im->Left();
        }
        else if (k == ovkRight)  {
            key->isShift() ? im->ShiftRight() : im->Right(); 
        }
        else if (k == ovkDown) { im->Down(); }
        else if (k == ovkEsc)  { im->Esc();  }
        else if (k == ovkTab)  { im->Tab();  }
        else if (k == ovkHome) { im->Home(); }
        else if (k == ovkEnd)  { im->End();  }
        else if (k == ovkDelete || k == ovkBackspace) { im->Backspace() ; }
        else if (k == ovkReturn) { im->Enter(); im->CommitReady(); }
        else { 
            DefaultKey(key,buf,textbar,srv);
        }
    }

    void DefaultKey(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
        if(key->isCtrl()) {
            if((key->code() >= '0') && (key->code() <= '9')) {
                im->CtrlNum(key->code());
            }
            else if(key->isAlt()) {
                im->CtrlOption(key->code());
            }
            return;
        }
        im->Key((key->isShift())?toupper(key->code()):tolower(key->code()));
    }

    void Capslock(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
        if(key->isCapslock()) {
            if(im->ChineseMode()) im->Capslock();
        }
        else if (!im->ChineseMode()) {
            im->Capslock();
        }
    }
    
    void Redraw(OVBuffer *buf, OVService *srv) {
        const char *s1,*s2,*s3;
        
        if(im->CommitReady()) {
            const char *s = im->CommitStr();
            buf->clear()->append(srv->toUTF8("big5", s))->send();
        }
        
        int ps=-1, pe=-1, ips=im->PointStart(), ipe=im->PointEnd();
        if (ips > -1 && ipe !=0) {
            if (ipe > 0) { ps=ips; pe=ps+ipe; }
            else { ps=ips+ipe; pe=ips; }
        }        
        
        // murmur("ips=%d, ipe=%d, ps=%d, pe=%d\n", ips, ipe, ps, pe);

        s1 = srv->toUTF8("big5", im->Buffer(0,im->CursorPos()-1));
        buf->clear()->append(s1);

        s2 = srv->toUTF8("big5", im->ZuinStr());
        buf->append(s2);
        
        s3 = srv->toUTF8("big5", im->Buffer(im->CursorPos()));
        buf->append(s3)->update(im->CursorPos(), ps, pe);
        // murmur("==> %s%s%s",s1,s2,s3);
    }
    
    void CandidateWindow(OVCandidate *textbar, OVService *srv) {
        if(im->Candidate()) {
            char s[64];
			char *ch, selkey;
            textbar->clear();
            for(int i=0; i < im->ChoicePerPage() ; i++) {
                ch      = im->Selection(i);
                selkey  = im->SelKey(i);
                if(ch[0]) {
                    char b[2];
                    sprintf(b, "%c.", selkey);
                    textbar->append((char *)b);
                    const char *cha = srv->toUTF8("big5", ch);
                    textbar->append(cha)->append(" ");
                }
                free(ch);
            }
            sprintf(s," %d/%d",im->CurrentPage() + 1,im->TotalPage());
            textbar->append((char*)s);
            textbar->update();
            textbar->show();
        }
        else {
            textbar->hide();
        }
    }

protected:
    OVIMChewing *parent;
    Chewing *im;
};


class OVIMChewing : public OVInputMethod {
public:
    OVIMChewing() {
        chew=NULL;
    }

    virtual ~OVIMChewing() {
        delete chew;
    }

    virtual int initialize(OVDictionary* l, OVService* s, const char* modulePath) {
        char chewingpath[PATH_MAX];
        char hashdir[PATH_MAX];
        
        // we can always be sure that an ending seperator '/' is given to us
        sprintf(hashdir, "%sOVIMChewing-OV0.7.0rc5", s->userSpacePath(identifier()));
        mkdir(hashdir, S_IRWXU);
        sprintf(chewingpath, "%sOVIMChewing", modulePath);
        if (!ChewingCheckData(chewingpath)) {
            murmur("OVIMChewing: chewing data missing at %s", modulePath);
            return 0;
        }
        
        murmur ("OVIMChewing: initialize, chewing data=%s, userhash=%s", chewingpath, hashdir);
		
		// BECAUSE THE {SACRILEGIOUS WORDS HERE} libchewing HAS NO 
		// EXCEPTION HANDLING HERE (BLAME OLD C-style assert() !!)
		// WE HAVE TO DO ERROR CHECKING OURSELVES, OTHERWISE WE ARE
        // DOOMED IF CHEWING DATA DOESN'T EXIST. THIS MAKES OUR LIFE
        // HARD BUT WE SHOULD TRY NOT TO COMPLAIN
        chew = new Chewing(chewingpath, hashdir);

        if(!l->keyExist("keyboardLayout")) l->setInteger("keyboardLayout", 0);
		chew->SetKeyboardLayout(l->getInteger("keyboardLayout"));
        return 1;
    }

    virtual void update(OVDictionary* localconfig, OVService*) {
        chew->SetKeyboardLayout(localconfig->getInteger("keyboardLayout"));
    }

    virtual const char *identifier() {
        return "OVIMChewing";
    }

    virtual const char *localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "酷音";
        if (!strcasecmp(locale, "zh_CN")) return "酷音";
 	    return "Chewing (smart phonetics)";
    }

    virtual OVInputMethodContext* newContext() {
	    return new OVIMChewingContext(this, chew); 
    }
    
protected:
    Chewing *chew;
};

// the module wrapper
OV_SINGLE_MODULE_WRAPPER(OVIMChewing);
