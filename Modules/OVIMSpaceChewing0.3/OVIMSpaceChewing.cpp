// OVIMChewing03.cpp
// This package is released under the Artistic License,
// please refer to LICENSE.txt for the terms of use and distribution

// #define OV_DEBUG

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
#include "chewing.h"

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

class OVIMChewing03;

class OVIMChewing03Context : public OVInputMethodContext 
{
public:
    OVIMChewing03Context(OVIMChewing03 *p, ChewingContext *ctx) {
        p=parent;
        im=ctx;
    }
	
    virtual void start(OVBuffer *key, OVCandidate *textbar, OVService *srv) {	
	} 
    virtual void clear() { chewing_handle_Enter(im); }
    virtual void end() { chewing_handle_Enter(im); }
     
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
        if(key->isCommand()) return 0;
        KeyPress(key,buf,textbar,srv);
        if(chewing_keystroke_CheckIgnore(im)) return 0;
        CandidateWindow(textbar, srv);
        Redraw(buf, srv);
	Notify(srv);
        return 1;
    }
  
protected:
    void KeyPress(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
        int k = key->code();
        if(k == ovkSpace) {
	   key->isShift() ? chewing_handle_ShiftSpace(im):chewing_handle_Space(im);
        }
	else if (k == ovkLeft)   {
	   key->isShift() ? chewing_handle_ShiftLeft(im):chewing_handle_Left(im);
        }
        else if (k == ovkRight)  {
           key->isShift() ?chewing_handle_ShiftRight(im):chewing_handle_Right(im);
        }
        else if (k == ovkDown) { chewing_handle_Down(im); }
        else if (k == ovkUp)   { chewing_handle_Up(im);   }
        else if (k == ovkEsc)  { chewing_handle_Esc(im);  }
        else if (k == ovkTab)  { chewing_handle_Tab(im);  }
        else if (k == ovkHome) { chewing_handle_Home(im); }
        else if (k == ovkEnd)  { chewing_handle_End(im); }
        else if (k == ovkDelete) { chewing_handle_Del(im); }
	else if (k == ovkBackspace) { chewing_handle_Backspace(im) ; }
        else if (k == ovkReturn) { chewing_handle_Enter(im); }
        else { 
            DefaultKey(key,buf,textbar,srv);
        }
    }

    void DefaultKey(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
        if(key->isCtrl()) {
            if((key->code() >= '0') && (key->code() <= '9')) {
	       chewing_handle_CtrlNum( im, (key->code()));
            } else if(key->isAlt()) { 
	       chewing_handle_CtrlOption(im, (key->code()));
	    }
            return;
        }
        chewing_handle_Default(im ,(key->isShift())?toupper(key->code()):tolower(key->code()));
    }

    void Redraw(OVBuffer *buf, OVService *srv) {
        if ( chewing_commit_Check( im ) ) {
                const char *s = chewing_commit_String( im );
                buf->clear()->append(s)->send();
        }
        int ps=-1, pe=-1;
	int ips= chewing_point_Start(im), ipe= chewing_point_End(im);
        if (ips > -1 && ipe !=0) {
            if (ipe > 0) { ps=ips; pe=ps+ipe; }
            else { ps=ips+ipe; pe=ips; }
        }
        murmur("ips=%d, ipe=%d, ps=%d, pe=%d\n", ips, ipe, ps, pe);
        const char *s1,*s2,*s3;
	int zuin_count;
	int pos = chewing_cursor_Current(im);
        murmur("==> %d\n", pos);
	s1= chewing_buffer_String( im, 0 , pos-1);
        buf->clear()->append(s1);
	s2= chewing_zuin_String( im, &zuin_count);
        buf->append(s2);
	s3= chewing_buffer_String_End( im, pos);
	buf->append(s3)->update(pos, ps, pe);
        murmur("==> %s%s%s",s1,s2,s3);
    }

    void Notify(OVService *srv) {
       if(chewing_aux_Length(im) != 0) {
	  char *msg = chewing_aux_String(im);
	  srv->notify(msg);
       }
    }
    
    void CandidateWindow(OVCandidate *textbar, OVService *srv) {
        if(chewing_cand_TotalPage(im) > 0) {

            char s[64];
	    int i=1;
	    char str[ 20 ];
	    char *cand_string;
            textbar->clear();
            chewing_cand_Enumerate( im );
	    while ( chewing_cand_hasNext( im ) ) {
	       if ( i == chewing_cand_ChoicePerPage( im ) )
		  break;
	       sprintf( str, "%c.", im->data->config.selKey[i - 1]);
	       textbar->append( str );
	       cand_string = chewing_cand_String( im );
	       sprintf( str, " %s ", cand_string );
	       textbar->append( str );
	       free( cand_string );
	       i++;
	    }
	    sprintf(s," %d/%d", chewing_cand_CurrentPage(im) + 1, chewing_cand_TotalPage(im));
	    textbar->append((char*)s); 
	    textbar->update();
	    textbar->show();
	}
	else {
	   textbar->hide();
	}
    }

protected:
    OVIMChewing03 *parent;
    ChewingContext *im;
};

char selKey_define[11] = "1234567890"; /* Default */

class OVIMChewing03 : public OVInputMethod {
   public:
      OVIMChewing03() {
	 ctx=NULL;
      }

      virtual ~OVIMChewing03() {
	 delete ctx;
      }

      virtual int initialize(OVDictionary* l, OVService* s, const char* modulePath) {
	 char chewingpath[PATH_MAX];
	 char hashdir[PATH_MAX];
	 ChewingConfigData config;

	 // no more need to create hash dir ourselves
	 sprintf(hashdir, "%s%s", s->userSpacePath(identifier()),
	       s->pathSeparator());

	 sprintf(chewingpath, "%sOVIMSpaceChewing03", modulePath);
	 if (!ChewingCheckData(chewingpath)) {
	    murmur("OVIMSpaceChewing: chewing data missing at %s", modulePath);
	    return 0;
	 }

	 murmur ("OVIMSpaceChewing: initialize, chewing data=%s, userhash=%s", chewingpath, hashdir);

	 // BECAUSE THE {SACRILEGIOUS WORDS HERE} libchewing HAS NO 
	 // EXCEPTION HANDLING HERE (BLAME OLD C-style assert() !!)
	 // WE HAVE TO DO ERROR CHECKING OURSELVES, OTHERWISE WE ARE
	 // DOOMED IF CHEWING DATA DOESN'T EXIST. THIS MAKES OUR LIFE
	 // HARD BUT WE SHOULD TRY NOT TO COMPLAIN
	 // chew = new Chewing(chewingpath, hashdir);
	 chewing_Init(chewingpath, hashdir);
	 ctx = chewing_new();

	 if(!l->keyExist("keyboardLayout")) l->setInteger("keyboardLayout", 0);
	 chewing_set_KBType( ctx, l->getInteger("keyboardLayout"));

	 config.selectAreaLen = 16;
	 config.maxChiSymbolLen = 16;
	 int i;
	 for ( i = 0; i < 10; i++ )
	    config.selKey[ i ] = selKey_define[ i ];
	 /* Enable configurations */
	 chewing_Configure( ctx, &config );

	 return 1;
      }

    virtual void update(OVDictionary* localconfig, OVService*) {
        chewing_set_KBType( ctx, localconfig->getInteger("keyboardLayout"));
    }

    virtual const char *identifier() {
        return "OVIMSpaceChewing03";
    }

    virtual const char *localizedName(const char *locale) {
        if (!strcasecmp(locale, "zh_TW")) return "酷音 0.3";
        if (!strcasecmp(locale, "zh_CN")) return "繁体酷音 0.3";
 	    return "Chewing (Smart Phonetics) 0.3";
    }

    virtual OVInputMethodContext* newContext() {
	    return new OVIMChewing03Context(this, ctx); 
    }
    
protected:
    ChewingContext *ctx;
};

// the module wrapper
OV_SINGLE_MODULE_WRAPPER(OVIMChewing03);
