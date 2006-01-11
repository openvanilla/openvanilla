// OVIMTibetan.h: The Tibetan Input Method
// 2004-2006 by Weizhong Yang 
//
// http://openvanilla.org
//
//  OpenVanilla Tibetan Input Method is based on the Sambhota Keyboard,
// please check http://iris.lib.virginia.edu/tibet/tools/jskad_docs/Sambhota_keymap_one.rtf for detail.
//
// This program is Zonble-ware and adopts Zonble's License
// -- "How could a person who doesn't even believe in law adopt any license?"

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
//		buf[len]=0;
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

#define SYMBOL_NUM 11
#define CONSONAT_NUM 35
#define VOWEL_NUM 8
#define FINALADD_NUM 2
#define MAX_COMPOSE 5
#define KEYBOARDS 4

/* 
 0: Sambhota Keymap One
 1: Sambhota Keymap Two
 2: TCC keyboard #1
 3: TCC keyboard #2
*/

char ComposeKey[KEYBOARDS] = {'f','h', 'h', 'a'};
char SpaceKey[KEYBOARDS] = {'.','-', '-', '-'};

char SymbolKeys[KEYBOARDS][SYMBOL_NUM] = 
{
	{'!', ',', '#', '$', '(', ')', '@', ':', ';','-', '|'},
	{'V', 'C', '@', '#', '(', ')', '|', ':', 'K','{', '&'},
	{'A', '\\', '!', '@', '(', ')', '$', ':', '%','+', '|'},
	{'A', '\\', '!', '@', '(', ')', '$', ':', '%','+', '|'}
};

unsigned short SymbolChars[SYMBOL_NUM] =
{
	0x0F00,0x0F0D,0x0F04,0x0F05,0x0F3C,0x0F3D,0x0F85,0x0F7F,0x0F14,0x0F11,0x0F08
};

int htransform = 28;

int htransformKey[5] = {
	2, 10, 14, 18, 32
};

unsigned short htransfromChars[5] = {
	0x0F43, 0x0f52, 0x0f57, 0x0f5C, 0x0f4D
};

char ConsonantKeys[KEYBOARDS][CONSONAT_NUM] = 
{
	{
	'k','K','g','G','c','C','j','N','t','T',
	'd','n','p','P','b','m','x','X','D','w',
	'Z','z','\'','y','r','l','S','s','h','A',
	'q','Q','v','V','B'
	},
	{
	'q','w','e','r','t','y','u','i','o','p',
	'[',']','a','s','d','f','k','l',';','\'',
	'z','x','c','v','m',',','.','/','>','?',
	'Q','W','E','R','T'
	},
	{
	'q','w','e','r','t','y','u','i','o','p',
	'[',']','a','s','d','f','k','l',';','\'',
	'z','x','c','v','m',',','.','/','>','?',
	'Q','W','E','R','T'
	},
	{
	'q','w','s','e','b','n','m',',','o','p',
	'j','k','r','/','d','f',';','\'','[',']',
	'z','x','c','g','h','v','.','l','G','H',
	'O','P','J','K','>'
	}
};

unsigned short ConsonantChars[CONSONAT_NUM] = 
{
	0x0F40,0x0F41,0x0F42,0x0F44,0x0F45,0x0F46,0x0F47,0x0F49,0x0F4F,0x0F50,
	0x0F51,0x0F53,0x0F54,0x0F55,0x0F56,0x0F58,0x0F59,0x0F5A,0x0F5B,0x0F5D,
	0x0F5E,0x0F5F,0x0F60,0x0F61,0x0F62,0x0F63,0x0F64,0x0F66,0x0F67,0x0F68,
	0x0F4A,0x0F4B,0x0F4C,0x0F4E,0x0F65
};

char VowelKeys[KEYBOARDS][VOWEL_NUM] =
{
	{'a','i','u','o','e','E','O','I'},
	{' ','g','j','n','b','B','N','G'},
	{' ','g','j','n','b','B','N','G'},
	{' ','t','u','i','y','Y','I','T'}
};

unsigned short VowelChars[VOWEL_NUM] = 
{
	0,0x0F72,0x0F74,0x0F7C,0x0F7A,0x0F7B,0x0F7D,0x0F80
}; 

char FinalAddKeys[KEYBOARDS][VOWEL_NUM]=
{ 
	{'%','&'},
	{'!','%'},
	{'`','*'},
	{'`','*'}
};

unsigned short FinalAddChars[FINALADD_NUM] = 
{
	0x0F83,0x0F7E
};

/* Wylie */

char *WylieConsonantKeys[] = 
{
	"k", "kh", "g", "ng", "c", "ch", "ja", "ny", "t", "th", 
	"d", "n", "p", "ph", "b", "m", "ts", "tsh", "dz", "w",
	"zh", "z", "'", "y", "r", "l", "sh", "s", "h", "a",
	"T", "Th", "D", "N", "Sh", "Dz", "f", "v"
};

unsigned short WylieConsonantChars[37] = 
{
	0x0F40,0x0F41,0x0F42,0x0F44,0x0F45,0x0F46,0x0F47,0x0F49,0x0F4F,0x0F50,
	0x0F51,0x0F53,0x0F54,0x0F55,0x0F56,0x0F58,0x0F59,0x0F5A,0x0F52,0x0F5D,
	0x0F5E,0x0F5F,0x0F60,0x0F61,0x0F62,0x0F63,0x0F64,0x0F66,0x0F67,0x0F68,
	0x0F4A,0x0F4B,0x0F4C,0x0F4E,0x0F57,0x0F55,0x0F56
};
