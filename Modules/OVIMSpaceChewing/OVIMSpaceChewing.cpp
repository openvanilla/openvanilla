// OVIMSpaceChewing: Chewing Module for OpenVanilla

// Originally written by Kang-min Liu, 2004-2005
// Maintained by zonble, 2005-2006
// Refactored by lukhnos, 2007

// This package is released under the Artistic License,
// please refer to LICENSE.txt for the terms of use and distribution

#define OV_DEBUG

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

#include <sys/stat.h>
#include <string>

#include "chewing.h"
#include "chewingio.h"

#ifdef LIBCHEWING_VERSION_OSX_INCLUDE
	#include "libchewing_version.h"
	#include <Carbon/Carbon.h>
#endif

#ifndef LIBCHEWING_VERSION
    #define LIBCHEWING_VERSION  "(trunk)"
#endif

using namespace std;


bool ChewingFileExist(const char *path, const char *file, const char *sep="/") {
    string fn;
    fn = path;
    fn += sep;
    fn += file;

    struct stat st;
    if (stat(fn.c_str(), &st)) return false;
    return true;
}

bool ChewingCheckData(const char *path) {
    char *files[8]={
        "ch_index.dat", 
        "dict.dat", 
        "fonetree.dat", 
        "ph_index.dat", 
        "pinyin.tab", 
        "swkb.dat", 
        "symbols.dat", 
        "us_freq.dat"
    };
    
    for (int i=0; i<8; i++) if (!ChewingFileExist(path, files[i])) return false;
    return true;
}

CHEWING_API char *chewing_buffer_String_old( ChewingContext *chewingContext, int from, int to )
{
	int i;
	char *s = (char *) calloc(
		1 + chewingContext->output->chiSymbolBufLen,
		sizeof(char) * MAX_UTF8_SIZE );
	if(from >= 0 && to < chewingContext->output->chiSymbolBufLen ) {
	   for ( i = from; i <= to; i++ ) {
	      strcat( s, (char *) (chewingContext->output->chiSymbolBuf[ i ].s) );
	   }
	}
	return s;
}

CHEWING_API char *chewing_buffer_String_End_old( ChewingContext *chewingContext, int from) {
	return chewing_buffer_String_old(chewingContext, from, chewingContext->output->chiSymbolBufLen -1 );
}

class OVIMSpaceChewing;

class OVIMSpaceChewingContext : public OVInputMethodContext 
{
public:
    OVIMSpaceChewingContext(ChewingContext *chewingContext) : im(chewingContext) {}
	
    virtual void start(OVBuffer *key, OVCandidate *textbar, OVService *srv)
	{
	}
	
    virtual void clear()
	{
		chewing_handle_Enter(im);
	}
	
    virtual void end()
	{
		chewing_handle_Enter(im);
	}
     
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv)
	{
		// fix the SHIFT-SPACE problem
		if (!textbar->onScreen() && buf->isEmpty() && key->isShift() && key->code() == 32) {
			buf->append("　")->send();
			return 1;
		}
				
        if(key->isCommand()) return 0;
		
        KeyPress(key,buf,textbar,srv);
        if(chewing_keystroke_CheckIgnore(im)) return 0;
        
		CandidateWindow(textbar, srv);
        Redraw(buf, srv);
		Notify(srv);
		
		if (textbar->onScreen() && buf->isEmpty() && 
			((key->isCtrl() && key->code() == '1') || (key->isCtrl() && key->code() == '0') || key->code() == '`'))
		{
			buf->append("符")->update();
		}
		
        return 1;
    }
  
protected:
    void KeyPress(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv)
	{
		int k = key->code();
		Capslock(key,buf,textbar,srv);
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
            }
			else if(key->isAlt()) { 
				#warning chewing_handle_CtrlOption not found in libchewing trunk
				// chewing_handle_CtrlOption(im, (key->code()));
			}
            return;
        }
        chewing_handle_Default(im ,(key->isShift())?toupper(key->code()):tolower(key->code()));
    }
	
    void Capslock(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar, OVService *srv) {
		if(key->isCapslock()) {
			if(chewing_get_ChiEngMode(im) == CHINESE_MODE) {
				chewing_handle_Capslock(im);
			}
		}
		else if (chewing_get_ChiEngMode(im) != CHINESE_MODE) {
			chewing_handle_Capslock(im);
       }
    }

    void Redraw(OVBuffer *buf, OVService *srv) {
		if ( chewing_commit_Check( im ) ) {
			const char *s = chewing_commit_String( im );
			buf->clear()->append(s)->send();
        }
		
        int ps=-1, pe=-1;
		
		// int ips= chewing_point_Start(im), ipe= chewing_point_End(im);
		int ips = im->output->PointStart;
		int ipe = im->output->PointEnd;
		
        if (ips > -1 && ipe !=0) {
            if (ipe > 0) {
				ps=ips;
				pe=ps+ipe;
			}
            else {
				ps=ips+ipe;
				pe=ips;
			}
        }
		
        // murmur("ips=%d, ipe=%d, ps=%d, pe=%d\n", ips, ipe, ps, pe);
		
        const char *s1,*s2,*s3;
		int zuin_count;
		
		#warning libchewing inconsistency in mod_aux.h and mod_aux.c, bug report needed
		int pos = (int)chewing_cursor_Current(im);
		
        // murmur("==> %d\n", pos);
		s1 = chewing_buffer_String_old( im, 0 , pos-1);
        buf->clear()->append(s1);
		
		s2= chewing_zuin_String( im, &zuin_count);
        buf->append(s2);
		
		s3= chewing_buffer_String_End_old( im, pos);
		buf->append(s3)->update(pos, ps, pe);
		
        //murmur("==> %s%s%s",s1,s2,s3);
    }

    void Notify(OVService *srv) {
       if(chewing_aux_Length(im) != 0) srv->notify(chewing_aux_String(im));
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
			   if ( i > chewing_cand_ChoicePerPage( im ) ) break;
			   
			   sprintf( str, "%c.", im->data->config.selKey[i - 1]);
			   textbar->append( str );
			   cand_string = chewing_cand_String( im );
			   sprintf( str, " %s ", cand_string );
			   textbar->append( str );
			   free( cand_string );
			   i++;
			   if( i == 5) textbar->append("\n");
			}
			sprintf(s," %d/%d", chewing_cand_CurrentPage(im) + 1, chewing_cand_TotalPage(im));
			textbar->append((char*)s)->update()->show();
		}
		else {
		   textbar->hide();
		}
    }

protected:
    OVIMSpaceChewing *parent;
    ChewingContext *im;
};

class OVIMSpaceChewing : public OVInputMethod
{
public:
	OVIMSpaceChewing() : chewingContext(NULL)
	{
	}

	virtual ~OVIMSpaceChewing()
	{
		delete chewingContext;
	}

	virtual int initialize(OVDictionary* l, OVService* s, const char* modulePath) {
		ChewingConfigData config;

        string hashdir;
        hashdir = s->userSpacePath(identifier());
        // hashdir += s->pathSeparator();
        
        string chewingpath;
		
		#ifdef LIBCHEWING_VERSION_OSX_INCLUDE
			CFBundleRef bundle = CFBundleGetBundleWithIdentifier(CFSTR("org.openvanilla.module.ovimspacechewing"));
			if (!bundle) return 0;
			
			CFURLRef url = CFBundleCopyResourcesDirectoryURL(bundle);
			if (!url) return 0;
			
			char buf[1024];
			CFURLGetFileSystemRepresentation(url, TRUE, (UInt8*)buf, sizeof(buf)-1);
			chewingpath = buf;
		#else
			chewingpath = modulePath;
			chewingpath += identifier();
		#endif
        
		if (!ChewingCheckData(chewingpath.c_str())) {
			murmur("OVIMSpaceChewing: chewing data missing at %s", chewingpath.c_str());
			return 0;
		}

		murmur ("OVIMSpaceChewing: Chewing data=%s, userhash=%s",
		  chewingpath.c_str(), hashdir.c_str());

		chewing_Init(chewingpath.c_str(), hashdir.c_str());
		chewingContext = chewing_new();

        // set default layout to Standard
        int kb = l->getIntegerWithDefault("keyboardLayout", 0);        
		chewing_set_KBType(chewingContext, kb);
		
		const char *selKey_define = "1234567890";
		
		if (kb==KB_HSU) selKey_define = "asdfjkl789";
		if (kb==KB_DVORAK_HSU) selKey_define = "aoeuhtn789";
		
		for (int i=0; i<MAX_SELKEY; i++) config.selKey[i] = selKey_define[i];
		

		// Enable this, because this is called... SpaceChewing
		config.bSpaceAsSelection = 1;
		config.candPerPage = 7;
		config.maxChiSymbolLen = 20;
		config.bAddPhraseForward = 0;
        // config.bEscCleanAllBuf = 0,

        // This seems to be removed
		// config.selectAreaLen = 14;
		
		// Set configurations
		chewing_Configure( chewingContext, &config );

		return 1;
	}

    virtual void update(OVDictionary* localconfig, OVService*)
	{
		chewing_set_KBType(chewingContext, localconfig->getInteger("keyboardLayout"));
    }

    virtual const char *identifier()
	{
        return "OVIMSpaceChewing";
    }

    virtual const char *localizedName(const char *locale)
	{
        if (!strcasecmp(locale, "zh_TW")) return "酷音 "LIBCHEWING_VERSION;
        if (!strcasecmp(locale, "zh_CN")) return "繁体酷音 "LIBCHEWING_VERSION;
 	    return "Chewing (Smart Phonetics) "LIBCHEWING_VERSION;
    }

    virtual OVInputMethodContext* newContext()
	{
	    return new OVIMSpaceChewingContext(chewingContext);
    }
    
protected:
    ChewingContext *chewingContext;
};

// the module wrapper
OV_SINGLE_MODULE_WRAPPER(OVIMSpaceChewing);
