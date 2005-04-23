// -*- mode: c++; c-basic-offset: 4; font-lock-mode: t -*-

#define OVDEBUG 1

/*
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"
#include "OpenVanilla/OVUtility.h"
#include "ChewingPP/Chewingpp.h" */

#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>  
#include <pwd.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>
#include <ChewingPP/Chewingpp.h>

class OVIMChewing;

class OVIMChewingContext : public OVInputMethodContext 
{
public:
    OVIMChewingContext(OVIMChewing *p, Chewing* chew) {p=parent; im=chew;}
    virtual ~OVIMChewingContext() {}
	//~OVIMChewingContext() {}
	
	virtual void clear() { im->Enter(); }
    virtual void end() { im->Enter(); }
     
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar,
						 OVService *srv) {
        if(key->isCommand()) return 0;
        KeyPress(key,buf,textbar,srv);
        if(im->KeystrokeIgnore()) return 0;
        CandidateWindow(textbar, srv);
        Redraw(buf, srv);
        return 1;
    }
  
    void KeyPress(OVKeyCode *key, OVBuffer *buf,
                  OVCandidate *textbar, OVService *srv) {
        int k = key->code();
        Capslock(key,buf,textbar,srv);
        if(k == ovkSpace) {
            key->isShift() ? im->ShiftSpace() : im->Space();
        } else if (k == ovkLeft)   {
            key->isShift() ? im->ShiftLeft()  : im->Left();
        } else if (k == ovkRight)  {
            key->isShift() ? im->ShiftRight() : im->Right(); 
        }
        else if (k == ovkDown)   { im->Down();  }
        else if (k == 27) { im->Esc();   }
        else if (k == ovkTab)    { im->Tab();   }
        else if (k == ovkHome)   { im->Home();  }
        else if (k == ovkEnd)    { im->End();   }
        else if (k == ovkDelete || k == ovkBackspace) { im->Backspace() ; }
        else if (k == ovkReturn  || k == 3)
            { im->Enter(); im->CommitReady(); }
        else {DefaultKey(key,buf,textbar,srv);}
    }

    void DefaultKey(OVKeyCode *key, OVBuffer *buf,
                    OVCandidate *textbar, OVService *srv) {
        if(key->isCtrl()) {
            if(key->code() >= 0 || key->code() <= 9) {
                im->CtrlNum(key->code());
            } else if(key->isOpt()) {
                im->CtrlOption(key->code());
            }
            return;
        }
        im->Key((key->isShift())?toupper(key->code()):tolower(key->code()));
    }

    void Capslock(OVKeyCode *key, OVBuffer *buf,
                  OVCandidate *textbar, OVService *srv) {
        if(key->isCapslock()) {
            if(im->ChineseMode()) im->Capslock();
        } else if (!im->ChineseMode()) {
            im->Capslock();
        }
    }
    
    void Redraw(OVBuffer *buf, OVService *srv) {
        const char *s1,*s2,*s3;
        
        if(im->CommitReady() ) {
            const char *s = im->CommitStr();
			s = srv->toUTF8("big5", s);
            buf->clear()->append(s)->send();
            free(s);
        }
        
        s1 = srv->toUTF8("big5", im->Buffer(0,im->CursorPos()-1));
        s2 = srv->toUTF8("big5", im->ZuinStr());
        s3 = srv->toUTF8("big5", im->Buffer(im->CursorPos()));
        
        buf->clear()->
            append(s1)->
            append(s2)->
            append(s3)->
            update(im->CursorPos(),-1,-1);
        
        murmur("==> %s%s%s",s1,s2,s3);
        free(s1); free(s2); free(s3);
    }
    
    void CandidateWindow(OVCandidate *textbar, OVService *srv) {
        if(im->Candidate()) {
            const char *s;
			char *ch,selkey;
            textbar->clear();
            for(int i=0; i < im->ChoicePerPage() ; i++) {
                ch      = im->Selection(i);
                selkey  = im->SelKey(i);
                if(ch[0]) {
                    sprintf(s,"%c. %s ",selkey,ch);
					s = srv->toUTF8("big5", s);
                    textbar->append(s);
                }
                free(ch);
            }
            sprintf(s," %d/%d",im->CurrentPage() + 1,im->TotalPage());
			s = srv->toUTF8("big5", s);
            textbar->append(s);
            textbar->update();
            textbar->show();
        } else {
            textbar->hide();
        }
    }

protected:
    OVIMChewing *parent;
    Chewing    *im;
};

char* mkHashDir() {
    char *ptr;
    static char hashdir[PATH_MAX];
    
    // this code is merged from SpaceChewingOSX, to solve SecurityAgent.app bug
    // this will be depracated as in the future userpath will be given us
    // by the Loader
    if (!(ptr=getenv("HOME")))
    {
        struct passwd *pw = getpwuid(getuid());
        if (pw == NULL) strcpy(hashdir, "/tmp");
        else strcpy(hashdir, pw->pw_dir);
    }
    else strcpy(hashdir, ptr);
        
    strcat(hashdir, "/Library/OpenVanilla") ;
    mkdir(hashdir, S_IRWXU) ;
    strcat(hashdir, "/OVIMChewing");
    mkdir(hashdir, S_IRWXU) ;
    
    return hashdir;
}

class OVIMChewing : public OVInputMethod
{
public:
    Chewing *chew;

    OVIMChewing() {
        murmur ("new IM instance created");
    }
    
    virtual ~OVIMChewing() {
        murmur ("IM instance destroyed");
        delete chew;
    }

    virtual int initialize(OVDictionary* l, OVService*, const char* modulePath)
    {
        char *hashdir = mkHashDir();
        char chewingpath[PATH_MAX];
        strcpy (chewingpath, modulePath);
        strcat (chewingpath, "OVIMChewing");
        
        //murmur ("OVIMChewing: creating ChewingPP instance, chewing data path=%s, whereas userhash data=%s", chewingpath, hashdir);
        printf("OVIMChewing: creating ChewingPP instance, chewing data path=%s, whereas userhash data=%s", chewingpath, hashdir);
		
        chew = new Chewing(chewingpath, hashdir);

        if(!l->keyExist("keyboardLayout"))
            l->setInteger("keyboardLayout", 0);
            
        chew->SetKeyboardLayout( l->getInteger("keyboardLayout") );
        return 1;
    }

    virtual int update(OVDictionary*, OVDictionary* localconfig)
    {
        int layout=localconfig->getInteger("keyboardLayout");
        chew->SetKeyboardLayout(layout);
        return 1; 
    }

    virtual const char *identifier()
	{
        return "OVIMChewing";
    }

    virtual const char *localizedName(const char *locale)
	{
        if (!strcasecmp(locale, "zh_TW"))
            return "OV 酷音";
        if (!strcasecmp(locale, "zh_CN"))
            return "OV 酷音";
		else
			return "OV Chewing";
    }

	virtual OVInputMethodContext* newContext()
	{
		return new OVIMChewingContext(this, chew); 
	}
};

// use these two wrappers
OV_SINGLE_MODULE_WRAPPER(OVIMChewing);
