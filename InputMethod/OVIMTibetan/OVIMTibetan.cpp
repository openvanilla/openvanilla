// OVIMTibetan.cpp 2004 by Weizhong Yang

#include <stdio.h>
#include <string.h>
#include "OpenVanilla/OpenVanilla.h"
#include "OpenVanilla/OVLoadable.h"

class OVTibetanIM;

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
char buf[ebMaxKeySeq];
int len;
};

char ConsonantKeys[37] = {
	'k','K','g','G','c','C','j','N','n','t',
	'T','d','D','p','P','b','B','m','x','X',
	'D','w','z','Z','\'','y','r', 'l', 'S','s',
	'h','A', 'v', 'q', 'Q', 'B', 'V'
};
unsigned short ConsonantChars[37] = {
	0x0F40,0x0F41,0x0F42,0x0F44,0x0F45,0x0F46,0x0F47,0x0F49,0x0F53,0x0F4F,
	0x0F50,0x0F51,0x0F52,0x0F54,0x0F55,0x0F56,0x0F57,0x0F58,0x0F59,0x0F5A,
	0x0F5B,0x0F5D,0x0F5F,0x0F5E,0x0F60,0x0F61,0x0F62,0x0F63,0x0F64,0x0F66,
	0x0F67,0x0F68,0x0F4C,0x0F4A,0x0F4B,0x0F65,0x0F4E
};

char VowelKeys[8] = {
	'a','i','u','o',
	'e','E','O','I'
};
unsigned short VowelChars[8] = {
	0,0x0F72,0x0F74,0x0F7C,
	0x0F7A,0x0F7B,0x0F7D,0x0F80
}; 

int isConsonantKey(int key){
	int i;
	for(i=0; i< 37; i++){
		if(key == ConsonantKeys[i])
			return i;
	}
	return -1;
}

int isVowelKey(int key){
	int i;
	for(i=0; i< 8; i++){
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
    
    virtual ~OVTibetanContext()
    {
    }
    
    virtual int activate(OVService *)
    {
		keyseq.clear();
        return 1;
    }
    
    virtual int deactivate(OVService *)
    {
        return 1;
    }
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
        OVService *srv)
    {
		
		if (key->isOpt() || key->isCommand() || key->isCtrl())
        {
			return 0;
        }
		    
        if (key->isCode(2, ovkReturn, ovkMacEnter))
        {
            if (!(strlen(keyseq.buf))) return 0;   // if buffer is empty, don't process
			buf->send()->clear();
			keyseq.clear();
			textbar->hide();
            return 1;   // key processed
        }
		
		if (key->isCode(1, ovkSpace)) //Space 輸出逗點
        {
			buf->append((char *)".")->send()->clear();
			keyseq.clear();
			textbar->hide();
            return 1;   // key processed
        }
		
		if (key->isCode(2, ovkDelete, ovkBackspace))
		{
			if(!strlen(keyseq.buf)) return 0;
			keyseq.remove();
			buf->update();
			return 1;
		}
		
        if (key->isPrintable())
        {
			unsigned short i;
			int isVowel = -1, isConsonant = -1, isConsonantPrev = -1;
			
			if(key->code() == '.'){ // 輸入的.，送出空白
				i = 0x0F20 + (key->code() - '0');
				buf->append((char *)" ")->send();
				return 1;   // key processed
			}
			
			if(key->code() == 'f'){ // 如果是 f
				if(keyseq.buf[0] == 'f' || keyseq.buf[0] == 'F') {
					buf->send()->clear();
					keyseq.clear();
					textbar->hide();
				} else{
					buf->send()->clear();
					keyseq.clear();
					keyseq.add('f');
					textbar->clear()->append((char *)"Composing...")->show();
				}
				return 1;   // key processed
			}
			
			if(key->code() >= '0' && key->code() <= '9'){ // 輸入的是數字，直接送出
				i = 0x0F20 + (key->code() - '0');
				buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
				keyseq.clear();
				textbar->hide();
				return 1;   // key processed
			}
			if((isVowel = isVowelKey(key->code())) > -1){ // 輸入的是母音
				if(keyseq.buf[0] &&
				   (isConsonantKey(keyseq.buf[0]) > -1 || keyseq.buf[0] == 'f')){
					i = VowelChars[isVowel];
					buf->append(&i, ovEncodingUTF16Auto, 1)->send()->clear();
					keyseq.clear();
					textbar->hide();
				}
				return 1;   // key processed
			}
			if((isConsonant = isConsonantKey(key->code())) > -1){ // 輸入的是子音
				if(keyseq.buf[0] == 'f') {
					if(strlen(keyseq.buf) < 4){
						i = ConsonantChars[isConsonant];
						if(strlen(keyseq.buf) > 1)
							i = i + 0x50;
						buf->append(&i, ovEncodingUTF16Auto, 1)->update();
						keyseq.add(key->code());
						return 1;
					} else {
					buf->send()->clear();
					keyseq.clear();
					textbar->hide();
					}
				} else if((isConsonantPrev = isConsonantKey(keyseq.buf[0])) > -1){
					//前一個也是子音，把前一個送出
					buf->send()->clear();
					keyseq.clear();
					textbar->hide();
				} 
				i = ConsonantChars[isConsonant];
				buf->append(&i, ovEncodingUTF16Auto, 1)->update();
				keyseq.add(key->code());
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
            return strlen(strcpy((char*)s, "OV 藏文 (Sambhota鍵盤)"));
        else if (!strcasecmp(locale, "zh_CN"))
            return strlen(strcpy((char*)s, "OV 藏文 (Sambhota键盘)"));
        else
            return strlen(strcpy((char*)s, "OV Tibetan(Sambhota Keyboard One)"));
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
