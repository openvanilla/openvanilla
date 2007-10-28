// OVIMTibetan.cpp: The Tibetan Input Method
// 2004-2007 by Weizhong Yang 
//
// http://openvanilla.org
//
//  OpenVanilla Tibetan Input Method is based on the Sambhota Keyboard,
// please check http://iris.lib.virginia.edu/tibet/tools/jskad_docs/Sambhota_keymap_one.rtf for detail.
//
// This program is Zonble-ware and adopts Zonble's License
// -- "How could a person who doesn't even believe in law adopt any license?"

// #define OV_DEBUG
#include <cstdio>
#include <cstring>
#include <cctype>

#ifndef WIN32
	#include <OpenVanilla/OpenVanilla.h>
	#include <OpenVanilla/OVLibrary.h>
	#include <OpenVanilla/OVUtility.h>
#else
	#include "OpenVanilla.h"
	#include "OVLibrary.h"
	#include "OVUtility.h"
#endif

#include "OVOSDef.h"

#include "OVIMTibetan.h"

int keyboardlayout = 0; 
// The Deafault keyboard layout is Sambhota Keymap #1
char *imlocale;

short isSymbolKey(int key)
// Check if the keycode is mapping to a punctuation or a symbol
{
   int i;
   for(i=0; i< SYMBOL_NUM; i++)
   {
      if(key == SymbolKeys[keyboardlayout][i])
	 return i;
   }
   return -1;
}

short isConsonantKey(int key)
{
   int i;
   for(i=0; i< CONSONAT_NUM; i++)
   {
      if(key == ConsonantKeys[keyboardlayout][i])
	 return i;
   }
   return -1;
}

short isVowelKey(int key)
{
   int i;
   for(i=0; i< VOWEL_NUM; i++)
   {
      if(key == VowelKeys[keyboardlayout][i])
	 return i;
   }
   return -1;
}

short isFinalAddKey(int key)
{
   int i;
   for(i=0; i< FINALADD_NUM; i++)
   {
      if(key == FinalAddKeys[keyboardlayout][i])
	 return i;
   }
   return -1;
}

short isHtransform(int key)
{
   int i;
   for(i=0; i< 5; i++)
   {
      if(key == htransformKey[i])
	 return i;
   }
   return -1;
}

class OVIMTibetan;

class OVTibetanContext : public OVInputMethodContext
{
   public:
      OVTibetanContext(OVIMTibetan *p){parent=p;}
      virtual ~OVTibetanContext(){}

      virtual void clear() {
	 keyseq.clear();
	 keyseq.lastisother();
      } 

      virtual void end() {
	 keyseq.clear();
	 keyseq.lastisother();
      }

      virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVCandidate *textbar,
	    OVService *srv)
      {
	 unsigned short i;
	 short j = -1, k = -1;
	 const char *u8;

	 if (key->isOpt() || key->isCommand() || key->isCtrl()){return 0;}
	 
	 if (key->code() == ovkUp || key->code() == ovkDown ||key->code() == ovkLeft ||key->code() == ovkRight ) //Locked when composing
	 { 
	    keyseq.lastisother();
	    if(keyseq.len) {
	        buf->send()->clear(); 
	    }
	    return 0;	// key processed
	 }

	 if (key->isCapslock()) { //CapLock
	    int code;
	    char b[2];
	    if (key->isShift()) { code = toupper(key->code()); }
	    else { code = tolower(key->code()); }
	    sprintf(b, "%c", code);
	    buf->append(b)->send()->clear();
	    return 1;
	 } 

	 if (key->code() == ovkReturn || key->code() == 3)
	 {
	    if (!keyseq.len) return 0;   // if buffer is empty, don't process
	    buf->send()->clear();
	    keyseq.clear();
	    keyseq.lastisother();
	    
	    return 1;   // key processed
	 }

	 if (key->code() == ovkSpace) //Space to keyin Tibetan seperator.
	 {
	    i = 0x0F0B;
	    u8=srv->UTF16ToUTF8(&i, 1);
	    buf->append(u8)->send()->clear();
	    keyseq.clear();
	    keyseq.lastisother();
	    
	    return 1;   // key processed
	 }

	 if (key->code() == ovkDelete || key->code() == ovkBackspace)
	 {
	    if(!keyseq.len) return 0;
	    buf->clear()->update();
	    keyseq.clear();
	    keyseq.lastisother();
	    
	    return 1;
	 }

	 {
	    char keycode = key->code();

	    if(keycode >= '0' && keycode <= '9')	// Numbers
	      // The numbric keys are always to input tibetan numbers 
	    { 
	       i = 0x0F20 + (keycode - '0');
	       u8=srv->UTF16ToUTF8(&i, 1);
	       buf->append(u8)->send()->clear();
	       keyseq.clear();
	       keyseq.lastisother();
	       return 1;   // key processed
	    }

	    if(keycode == SpaceKey[keyboardlayout]) // Keyin a space.
	    { 
	       buf->append((char *)" ")->send();
	       keyseq.clear();
	       keyseq.lastisother();
	       return 1;   // key processed
	    }

	    if(keycode == ComposeKey[keyboardlayout])	// Compose key
	    { 
	       buf->send()->clear();
	       if(keyseq.buf[0] == ComposeKey[keyboardlayout]) //End Stacking
	       {
		  keyseq.clear();
		  keyseq.lastisconsonant();
	       } else { //Begin Composing
		  keyseq.clear();
		  keyseq.lastisother();
		  keyseq.add(keycode);
		  if (!strcasecmp(srv->locale(), "zh_TW")) 
			  srv->notify(
				"\xE9\x80\xB2\xE5\x85\xA5\xE7\xB5\x84\xE5\x90\x88\xE5\xAD\x97\xE7\x8B\x80\xE6\x85\x8B\xE2\x80\xA6");
		  else if(!strcasecmp(srv->locale(), "zh_CN"))
			  srv->notify(
				"\xE8\xBF\x9B\xE5\x85\xA5\xE7\xBB\x84\xE5\x90\x88\xE5\xAD\x97\xE7\x8A\xB6\xE6\x80\x81\xE2\x80\xA6");
		  else
			  srv->notify("Stacking characters..");
	       }
	       buf->update();
	       return 1;   // key processed
	    }

	    if((j = isFinalAddKey(keycode)) > -1)	// Final M or N
	    {
	       if(keyseq.buf[0]== ComposeKey[keyboardlayout] && keyseq.len == 1) return 1;
	       if(keyseq.last || keyseq.len){
		  i = FinalAddChars[j];
		  u8=srv->UTF16ToUTF8(&i, 1);
		  buf->append(u8)->send()->clear();
		  keyseq.clear();
		  keyseq.lastisother();
		  
	       }
	       return 1;
	    }

	    if((j = isSymbolKey(keycode)) > -1) // Symbols
	    {
	       i = SymbolChars[j];
	       u8=srv->UTF16ToUTF8(&i, 1);
	       buf->append(u8)->send()->clear();
	       keyseq.clear();
	       keyseq.lastisother();
	       
	       return 1;   // key processed
	    }

	    if((j = isVowelKey(keycode)) > -1)	// Vowels
	    { 
	       if(keyseq.last == 1 || (keyseq.buf[0] == ComposeKey[keyboardlayout] && keyseq.len > 3))
	       {
		  i = VowelChars[j];
		  u8=srv->UTF16ToUTF8(&i, 1);
		  buf->append(u8)->send()->clear();
		  keyseq.clear();
		  keyseq.lastisvowel();
		  
	       }
	       return 1;   // key processed
	    }

	    if(keyboardlayout == 1) //Sambhota Keymap #2
	    {
	       if(keycode == 'D') keycode = 'm';
	       if(keycode == 'F') keycode = '\'';
	    }

	    if((j = isConsonantKey(keycode)) > -1) // Consonant
	    { 
	       i = ConsonantChars[j];
	       keyseq.lastisconsonant();
	       if(keyseq.buf[0] == ComposeKey[keyboardlayout] && keyseq.len < MAX_COMPOSE)  // Stacking 
	       {
		  if(j == 34 && keyseq.len > 1) //kb transform
		  { 
		     if(isConsonantKey(keyseq.buf[keyseq.len -1]) == 0)
		     {
			buf->clear()->update();
			if(keyseq.len < 3)
			{
			   i = 0x0F69;
			   u8=srv->UTF16ToUTF8(&i, 1);
			   buf->append(u8)->update();
			} else 
			{
			   if(((k = isHtransform(isConsonantKey(keyseq.buf[1]))) > -1) &&
				 isConsonantKey(keyseq.buf[2]) == htransform)
			   {
			      i = htransfromChars[k];
			      u8=srv->UTF16ToUTF8(&i, 1);
			      buf->append(u8);
			   } 
			   else if (isConsonantKey(keyseq.buf[1]) == 0 &&  //ka
				 isConsonantKey(keyseq.buf[2]) == 34)   //ssa
			   {
			      i = 0x0FB9;
			      u8=srv->UTF16ToUTF8(&i, 1);
			      buf->append(u8);	//kssa
			   } 
			   else 
			   {
			      i = ConsonantChars[isConsonantKey(keyseq.buf[1])];
			      u8=srv->UTF16ToUTF8(&i, 1);
			      buf->append(u8);
			      if(keyseq.len == 4)
			      {
				 i = ConsonantChars[isConsonantKey(keyseq.buf[2])];
				 u8=srv->UTF16ToUTF8(&i, 1);
				 buf->append(u8);
			      }
			   }
			   i = 0x0FB9;
			   u8=srv->UTF16ToUTF8(&i, 1);
			   buf->append(u8)->update();
			}
			keyseq.add(keycode);
			return 1;
		     }
		  }

		  if(j == htransform && keyseq.len > 1) //H transform
		  { 
		     if((k = isHtransform(isConsonantKey(keyseq.buf[keyseq.len -1]))) > -1)
		     {
			buf->clear()->update();
			if(keyseq.len < 3)
			{
			   i = htransfromChars[k];
			   u8=srv->UTF16ToUTF8(&i, 1);
			   buf->append(u8)->update();
			} else {
			   int l = -1;
			   if(((l = isHtransform(isConsonantKey(keyseq.buf[1]))) > -1) &&
				 isConsonantKey(keyseq.buf[2]) == htransform)
			   {
			      i = htransfromChars[l];
			      u8=srv->UTF16ToUTF8(&i, 1);
			      buf->append(u8);
			   } 
			   else if (isConsonantKey(keyseq.buf[1]) == 0 &&	//ka
				 isConsonantKey(keyseq.buf[2]) == 34)	//ssa
			   {
			      i = 0x0FB9;
			      u8=srv->UTF16ToUTF8(&i, 1);
			      buf->append(u8);
			   } 
			   else 
			   {
			      i = ConsonantChars[isConsonantKey(keyseq.buf[1])];
			      u8=srv->UTF16ToUTF8(&i, 1);
			      buf->append(u8);
			      if(keyseq.len == 4)
			      {
				 i = ConsonantChars[isConsonantKey(keyseq.buf[2])];
				 u8=srv->UTF16ToUTF8(&i, 1);
				 buf->append(u8);
			      }
			   }
			   i = htransfromChars[k] + 0x50;
			   u8=srv->UTF16ToUTF8(&i, 1);
			   buf->append(u8)->update();
			}
			keyseq.add(keycode);
			return 1;
		     }
		  }
		  if(keyseq.len > 1) 
		  {
		     if(j == 22) 
		     {
			i = 0x0F71;
		     } else {
			i = i + 0x50; // Sub characters.
		     }
		  }
		  u8=srv->UTF16ToUTF8(&i, 1);
		  buf->append(u8)->update();
		  keyseq.add(keycode);
		  return 1;
	       }
	       u8=srv->UTF16ToUTF8(&i, 1);
	       buf->append(u8)->send()->clear();
	       keyseq.clear();
	       
	       return 1;   // key processed
	    }
	    { //Extra 
	       if(keycode == 'M' || keycode == '>')
	       {
		  buf->clear()->append((char*)"\xE0\xBD\xA7\xE0\xBD\xB1\xE0\xBD\xB4\xE0\xBE\x83")->send();keyseq.clear();
	       }
	       if(keycode == '+' || keycode == '!')
	       {
		  buf->clear()->append((char*)"\xE0\xBC\x84\xE0\xBC\x85")->send();keyseq.clear();
	       }
	       if(keycode == '=')
	       {
		  buf->clear()->append((char*)"\xE0\xBD\xA8\xE0\xBD\xB1\xE0\xBD\xBF")->send();keyseq.clear();
	       }
	       if(keycode == '<')
	       {
		  buf->clear()->append((char*)"\xE0\xBD\xA8\xE0\xBD\xBC\xE0\xBD\xBE")->send();keyseq.clear();
	       }
	       if(keycode == '^')
	       {
		  buf->clear()->append((char*)"\xE0\xBC\x81\xE0\xBE\x83")->send();keyseq.clear();
	       }
	    }
	    return 1;
	 }        
	 return 0;   // key not processed
      }

   protected:
      OVIMTibetan *parent;
      KeySeq keyseq;
};

class OVIMTibetan : public OVInputMethod
{
   public:
      OVIMTibetan(){}
      virtual const char *identifier()
      {
	 return "OVIMTibetan";
      }

      virtual const char *localizedName(const char *locale)
      {
	 if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
	    return "\xE8\x97\x8F\xE6\x96\x87";
	 else
	    return "Tibetan";
      }

      virtual int initialize(OVDictionary* l, OVService*, const char* modulePath)
      {
	 if (!l->keyExist("keyboardLayout")) l->setInteger("keyboardLayout", 0);
	 keyboardlayout=l->getInteger("keyboardLayout");
	 if(keyboardlayout >= KEYBOARDS)
	    keyboardlayout = 0;
	 return 1;
      }

      virtual int update(OVDictionary*, OVDictionary*)
      {
	 return 1;
      }

      virtual OVInputMethodContext* newContext() 
      {
	 return new OVTibetanContext(this); 
      }
};


// use these two wrappers
OV_SINGLE_MODULE_WRAPPER(OVIMTibetan);
