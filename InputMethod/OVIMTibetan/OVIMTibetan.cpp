// OVIMTibetan.cpp 2004 by Weizhong Yang
/* OpenVanilla Tibetan Input Method is based on the Sambhota Keyboard, please check
http://iris.lib.virginia.edu/tibet/tools/jskad_docs/Sambhota_keymap_one.rtf for detail. */

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"

class OVTibetanIM;

#define VOWEL 2
#define CONSONAT 1
#define OTHER 0

const int ebMaxKeySeq=10;
class KeySeq
{
public:
    KeySeq() { len=0; buf[0]=0; }
    void add(char c)
{
	if (len == ebMaxKeySeq) return;
	buf[len++]=c;
	buf[len]=0;
}
void remove()
{
	if (!len) return;
	buf[--len]=0;
}
void clear() { len=0; buf[0]=0; }
void lastisvowel() {last=VOWEL;}
void lastisconsonant() {last=CONSONAT;}
void lastisother() {last=OTHER;}
char buf[ebMaxKeySeq];
int len;
int last;
};

#define SYMBOL_NUM 10
#define CONSONAT_NUM 37
#define VOWEL_NUM 8

char ComposeKey = 'f';

char SymbolKeys[SYMBOL_NUM] = 
{
	'!', ',', '#', '$', '(', ')', '@', ':', ';','-'
};

unsigned short SymbolChars[SYMBOL_NUM] =
{
	0x0F00,0x0F0D,0x0F04,0x0F05,0x0F3C,0x0F3D,0x0F85,0x0F7F,0x0F14,0x0F11
};

char ConsonantKeys[CONSONAT_NUM] = 
{
	'k','K','g','G','c','C','j','N','n','t',
	'T','d','D','p','P','b','B','m','x','X',
	'D','w','z','Z','\'','y','r', 'l', 'S','s',
	'h','A', 'v', 'q', 'Q', 'B', 'V'
};

unsigned short ConsonantChars[CONSONAT_NUM] = 
{
	0x0F40,0x0F41,0x0F42,0x0F44,0x0F45,0x0F46,0x0F47,0x0F49,0x0F53,0x0F4F,
	0x0F50,0x0F51,0x0F52,0x0F54,0x0F55,0x0F56,0x0F57,0x0F58,0x0F59,0x0F5A,
	0x0F5B,0x0F5D,0x0F5F,0x0F5E,0x0F60,0x0F61,0x0F62,0x0F63,0x0F64,0x0F66,
	0x0F67,0x0F68,0x0F4C,0x0F4A,0x0F4B,0x0F65,0x0F4E
};

char VowelKeys[VOWEL_NUM] = 
{
	'a','i','u','o',
	'e','E','O','I'
};

unsigned short VowelChars[VOWEL_NUM] = 
{
	0,0x0F72,0x0F74,0x0F7C,
	0x0F7A,0x0F7B,0x0F7D,0x0F80
}; 

short isSymbolKey(int key)
{
	int i;
	for(i=0; i< SYMBOL_NUM; i++)
	{
		if(key == SymbolKeys[i])
			return i;
	}
	return -1;
}

short isConsonantKey(int key)
{
	int i;
	for(i=0; i< CONSONAT_NUM; i++)
	{
		if(key == ConsonantKeys[i])
			return i;
	}
	return -1;
}

short isVowelKey(int key)
{
	int i;
	for(i=0; i< VOWEL_NUM; i++)
	{
		if(key == VowelKeys[i])
			return i;
	}
	return -1;
}

class OVTibetanContext : public OVIMContext
{
public:
    OVTibetanContext(OVTibetanIM *p)
    {
        parent=p;
        fprintf (stderr, "New IM context created\n");
    }
    
    virtual ~OVTibetanContext(){}
    
    virtual int activate(OVService *)
    {
		keyseq.clear();
		keyseq.lastisother();
        return 1;
    }
    
    virtual int deactivate(OVService *)
    {
        return 1;
    }
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
		unsigned short i;
		short j = -1;
		
		if (key->isOpt() || key->isCommand() || key->isCtrl())
        {
			return 0;
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
		
		if (key->isCode(4, ovkUp, ovkDown, ovkLeft, ovkRight)) //Lock when composing
		{ 
			if(!keyseq.len) return 0;
			keyseq.lastisother();
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
			if (key->isCapslock()) {
				buf->appendChar(key->lower())->send();
				return 1;
			}
			if(key->code() == '.') // Keyin a space.
			{ 
				buf->append((char *)" ")->send();
				keyseq.clear();
				keyseq.lastisother();
				textbar->hide();
				return 1;   // key processed
			}
			
			if(key->code() == ComposeKey)	// Compose key
			{ 
				buf->send()->clear();
				keyseq.clear();
				if(keyseq.buf[0] == ComposeKey) //End Composing
				{
					keyseq.lastisconsonant();
					textbar->hide();
				} else { //Begin Composing
					keyseq.lastisother();
					keyseq.add(ComposeKey);
					textbar->clear()->append((char *)"Composing...")->show();
				}
				return 1;   // key processed
			}
			
			if(key->code() == '%' || key->code() == '&')	// 輸入的是 % 或是 &
			{
				if(keyseq.last || keyseq.len){
					i = (key->code() == '%') ? 0x0F82 : 0x0F7E;
					buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
					keyseq.clear();
					keyseq.lastisother();
					textbar->hide();
				}
			}
			if(key->code() >= '0' && key->code() <= '9')	// Numbers
			{ 
				i = 0x0F20 + (key->code() - '0');
				buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
				keyseq.clear();
				keyseq.lastisother();
				textbar->hide();
				return 1;   // key processed
			}
			if((j = isSymbolKey(key->code())) > -1) // Symbols
			{
				i = SymbolChars[j];
				buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
				keyseq.clear();
				keyseq.lastisother();
				textbar->hide();
				return 1;   // key processed
			}
			if((j = isVowelKey(key->code())) > -1)	// Vowels
			{ 
				if(keyseq.last == 1 || (keyseq.buf[0] == ComposeKey && keyseq.len > 3))
				{
					i = VowelChars[j];
					buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
					keyseq.clear();
					keyseq.lastisvowel();
					textbar->hide();
				}
				return 1;   // key processed
			}
			if((j = isConsonantKey(key->code())) > -1) // Consonant
			{ 
				i = ConsonantChars[j];
				keyseq.lastisconsonant();
				if(keyseq.buf[0] == ComposeKey && keyseq.len < 4) 
				{
					if(keyseq.len > 1)
						i = i + 0x50; // Sub characters.
					buf->append(&i, ovEncodingUTF16Auto, 1)->update();
					keyseq.add(key->code());
					return 1;
				} 
				buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
				keyseq.clear();
				textbar->hide();
				return 1;   // key processed
			}
			return 1;
        }        
        return 0;   // key not processed
    }
    
protected:
    OVTibetanIM *parent;
	KeySeq keyseq;
};

class OVTibetanIM : public OVInputMethod
{
public:
    OVTibetanIM(){}
    virtual ~OVTibetanIM(){}
    virtual int identifier(char* s)
    {
        return strlen(strcpy(s, "OVTibetanIM"));
    }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        if (!strcasecmp(locale, "zh_TW"))
            return strlen(strcpy((char*)s, "OV 藏文(Sambhota Keymap One)"));
        else if (!strcasecmp(locale, "zh_CN"))
            return strlen(strcpy((char*)s, "OV 藏文(Sambhota Keymap One)"));
        else
            return strlen(strcpy((char*)s, "OV Tibetan(Sambhota Keymap One)"));
    }

    virtual int initialize(OVDictionary*, OVDictionary*, OVService*, char*)
    {
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
OVLOADABLEWRAPPER(OVTibetanIM)
OVLOADABLECANUNLOAD