// OVIMTibetan.cpp 2004 by Weizhong Yang
/* OpenVanilla Tibetan Input Method is based on the Sambhota Keyboard, please check
http://iris.lib.virginia.edu/tibet/tools/jskad_docs/Sambhota_keymap_one.rtf for detail. */

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"
#include "OVIMTibetan.h"

class OVIMTibetan;
int keyboardlayout = 0;
char *imlocale;

short isSymbolKey(int key)
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

class OVTibetanContext : public OVIMContext
{
public:
    OVTibetanContext(OVIMTibetan *p){parent=p;}
    virtual ~OVTibetanContext(){}
    
    virtual int activate(OVService *)
    {
		keyseq.clear();
		keyseq.lastisother();
		return 1;
    }
    
    virtual int deactivate(OVService *)
    {
		keyseq.clear();
		keyseq.lastisother();
		return 1;
    }

    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
		unsigned short i;
		short j = -1, k = -1;
							
		if (key->isOpt() || key->isCommand() || key->isCtrl()){return 0;}
		
		if (key->isCapslock()) { //CapLock
			if (key->isShift()) buf->appendChar(key->upper());
			else buf->appendChar(key->lower())->send();
			return 1;
		}
		    
        if (key->isCode(2, ovkReturn, ovkMacEnter))
        {
            if (!keyseq.len) return 0;   // if buffer is empty, don't process
			buf->send()->clear();
			keyseq.clear();
			keyseq.lastisother();
			textbar->hide();
            return 1;   // key processed
        }
		
		if (key->isCode(4, ovkUp, ovkDown, ovkLeft, ovkRight)) //Locked when composing
		{ 
			keyseq.lastisother();
			if(!keyseq.len) return 0;
			return 1;	// key processed
		}
		
		if (key->isCode(1, ovkSpace)) //Space to keyin Tibetan seperator.
        {
			i = 0x0F0B;
			buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
			keyseq.clear();
			keyseq.lastisother();
			textbar->hide();
            return 1;   // key processed
        }
		
		if (key->isCode(2, ovkDelete, ovkBackspace))
		{
			if(!keyseq.len) return 0;
			buf->clear()->update();
			keyseq.clear();
			keyseq.lastisother();
			textbar->hide();
			return 1;
		}
		
		
        if (key->isPrintable())
        {
			/* fprintf (stderr, "keysེeq: %s, len: %d, keycode: %c \n", keyseq.buf, keyseq.len, key->code()); */
			char keycode = key->code();

			if(keycode >= '0' && keycode <= '9')	// Numbers
			{ 
				i = 0x0F20 + (keycode - '0');
				buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
				keyseq.clear();
				keyseq.lastisother();
				textbar->hide();
				return 1;   // key processed
			}
			
			if(keycode == SpaceKey[keyboardlayout]) // Keyin a space.
			{ 
				buf->append((char *)" ")->send();
				keyseq.clear();
				keyseq.lastisother();
				textbar->hide();
				return 1;   // key processed
			}
			
			if(keycode == ComposeKey[keyboardlayout])	// Compose key
			{ 
				buf->send()->clear();
				if(keyseq.buf[0] == ComposeKey[keyboardlayout]) //End Stacking
				{
					keyseq.clear();
					keyseq.lastisconsonant();
					textbar->hide();
				} else { //Begin Composing
					keyseq.clear();
					keyseq.lastisother();
					keyseq.add(keycode);
					textbar->clear()->append((char *)"Stacking...")->show();
				}
				return 1;   // key processed
			}
			
			if((j = isFinalAddKey(keycode)) > -1)	// Final M or N
			{
				if(keyseq.buf[0]=='f' && keyseq.len == 1) return 1;
				if(keyseq.last || keyseq.len){
					i = FinalAddChars[j];
					buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
					keyseq.clear();
					keyseq.lastisother();
					textbar->hide();
				}
				return 1;
			}

			if((j = isSymbolKey(keycode)) > -1) // Symbols
			{
				i = SymbolChars[j];
				buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
				keyseq.clear();
				keyseq.lastisother();
				textbar->hide();
				return 1;   // key processed
			}

			if((j = isVowelKey(keycode)) > -1)	// Vowels
			{ 
				if(keyseq.last == 1 || (keyseq.buf[0] == ComposeKey[keyboardlayout] && keyseq.len > 3))
				{
					i = VowelChars[j];
					buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
					keyseq.clear();
					keyseq.lastisvowel();
					textbar->hide();
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
								buf->append(&i, ovEncodingUTF16Auto, 1)->update();
							} else 
							{
								if(((k = isHtransform(isConsonantKey(keyseq.buf[1]))) > -1) &&
								   isConsonantKey(keyseq.buf[2]) == htransform)
								{
									i = htransfromChars[k];
									buf->append(&i, ovEncodingUTF16Auto, 1);
								} 
								else if (isConsonantKey(keyseq.buf[1]) == 0 &&  //ka
										 isConsonantKey(keyseq.buf[2]) == 34)   //ssa
								{
									i = 0x0FB9;
									buf->append(&i, ovEncodingUTF16Auto, 1);	//kssa
								} 
								else 
								{
									i = ConsonantChars[isConsonantKey(keyseq.buf[1])];
									buf->append(&i, ovEncodingUTF16Auto, 1);
									if(keyseq.len == 4)
									{
										i = ConsonantChars[isConsonantKey(keyseq.buf[2])];
										buf->append(&i, ovEncodingUTF16Auto, 1);
									}
								}
								i = 0x0FB9;
								buf->append(&i, ovEncodingUTF16Auto, 1)->update();
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
								buf->append(&i, ovEncodingUTF16Auto, 1)->update();
							} else {
								int l = -1;
								if(((l = isHtransform(isConsonantKey(keyseq.buf[1]))) > -1) &&
								    isConsonantKey(keyseq.buf[2]) == htransform)
								{
									i = htransfromChars[l];
									buf->append(&i, ovEncodingUTF16Auto, 1);
								} 
								else if (isConsonantKey(keyseq.buf[1]) == 0 &&	//ka
										 isConsonantKey(keyseq.buf[2]) == 34)	//ssa
								{
									i = 0x0FB9;
									buf->append(&i, ovEncodingUTF16Auto, 1);	//kssa
								} 
								else 
								{
									i = ConsonantChars[isConsonantKey(keyseq.buf[1])];
									buf->append(&i, ovEncodingUTF16Auto, 1);
									if(keyseq.len == 4)
									{
										i = ConsonantChars[isConsonantKey(keyseq.buf[2])];
										buf->append(&i, ovEncodingUTF16Auto, 1);
									}
								}
								i = htransfromChars[k] + 0x50;
								buf->append(&i, ovEncodingUTF16Auto, 1)->update();
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
					buf->append(&i, ovEncodingUTF16Auto, 1)->update();
					keyseq.add(keycode);
					return 1;
				} 
				buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
				keyseq.clear();
				textbar->hide();
				return 1;   // key processed
			}
			{ //Extra 
				if(keycode == 'M' || keycode == '>')
				{
					buf->clear()->append((char*)"ཧཱུྃ")->send();keyseq.clear();textbar->hide();
				}
				if(keycode == '+' || keycode == '!')
				{
					buf->clear()->append((char*)"༄༅")->send();keyseq.clear();textbar->hide();
				}
				if(keycode == '=')
				{
					buf->clear()->append((char*)"ཨཱཿ")->send();keyseq.clear();textbar->hide();
				}
				if(keycode == '<')
				{
					buf->clear()->append((char*)"ཨོཾ")->send();keyseq.clear();textbar->hide();
				}
				if(keycode == '^')
				{
					buf->clear()->append((char*)"༁ྃ")->send();keyseq.clear();textbar->hide();
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
    virtual ~OVIMTibetan(){}
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVIMTibetan"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
		imlocale = locale;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OV 藏文"));
        else if (!strcasecmp(locale, "zh_CN"))
            return strlen(strcpy((char*)s, "OV 藏文"));
        else
            return strlen(strcpy((char*)s, "OV Tibetan"));
    }

    virtual int initialize(OVDictionary* g, OVDictionary* l, OVService*, char*)
    {
		if (!l->keyExist("keyboardLayout")) l->setInt("keyboardLayout", 0);
		keyboardlayout=l->getInt("keyboardLayout");
		if(keyboardlayout >= KEYBOARDS)
			keyboardlayout = 0;
        return 1;
    }
    
    virtual int terminate(OVDictionary*, OVDictionary*, OVService*)
    {
        return 1;
    }

    virtual int update(OVDictionary*, OVDictionary*)
    {
        return 1;
    }

    virtual OVIMContext* newContext() 
    {
        return new OVTibetanContext(this); 
    }
    
    virtual void deleteContext(OVIMContext* s) 
    {
        delete s; 
    }
};


// use these two wrappers
OVLOADABLEWRAPPER(OVIMTibetan)
OVLOADABLECANUNLOAD
