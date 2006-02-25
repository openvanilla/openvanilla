#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

bool POJIsCharInList(char k, const char *charlist) {
    size_t s=strlen(charlist);
    for (size_t i=0; i<s; i++) if (charlist[i]==k) return true;
    return false;
}

char POJLastChar(const string &s) {
    const char *cs=s.c_str();
    if (strlen(cs)) {
        return cs[strlen(cs)-1];
    }
    return 0;
}


const char* POJ_VOWEL="aeiouAEIOU";

class POJSyllable_Holo {
protected:
    vector<string> syllable;

public:
    POJSyllable_Holo() {
        clear();
    }

    virtual ~POJSyllable_Holo() {
    }
    
    virtual bool isEmpty();
    virtual void clear();
    virtual bool isValidKey(char k);
    virtual bool add(char k);
    virtual void backspace();       // remove the last character

    virtual const string queryString();     // get the string for db query


    virtual void normalize();       // apply simplification rules etc.
    
    
    
    virtual const string composite();    

    virtual void dump();            // debug dump
};

const string POJSyllable_Holo::queryString() {
    int tone=0;
    string qs;
    
    for (size_t i=0; i<syllable.size(); i++) {
        const char *s=syllable[i].c_str();
        
        // scan tone if there isn't none yet
        for (size_t j=0; j<strlen(s); j++) {
            switch (s[j]) {
                case '\'': if (!tone) tone=2; break;
                case '`': if (!tone) tone=3; break;
                case '^': if (!tone) tone=5; break;
                case '=': if (!tone) tone=7; break;
                case '|': if (!tone) tone=8; break;
                default: 
                    qs+=s[j];
            }
        }
    }
    

    if (tone) qs += ('0' + tone);

    return qs;
}

void POJSyllable_Holo::dump() {
    for (size_t i=0; i<syllable.size(); i++) {
        cout << i << "=" << syllable[i] << endl;
    }
    cout << "query string = " << queryString() << endl;
}

bool POJSyllable_Holo::isEmpty() {
    if (syllable.size()) return false;
    return true;
}

void POJSyllable_Holo::clear() {
    syllable.clear();
}

bool POJSyllable_Holo::isValidKey(char k) {
    return true;
}

bool POJSyllable_Holo::add(char k) {
    if (!isValidKey(k)) return false;

    // IF LAYOUT==TONEMARK_BEFORE_VOWEL && k==TONEMARK, enter wait state

    size_t s=syllable.size();    
    if (s) {    // if there is already something in the syllable
        int li=s-1;     // lastindex = size-1;
        string last=syllable[li];
        
        // <TONEMARK-RULE>: combines tonemark
        if (POJIsCharInList(POJLastChar(last), "'`^=|")) {
            syllable[li]+=k;
            return true;
        }
        
        // <OU-RULE>: combines preceeding "o" and incoming key "u"
        if (k=='u' || k=='U') {
            if (POJLastChar(last)=='O') { syllable[li]+="U"; return true; }
            if (POJLastChar(last)=='o') { syllable[li]+="u"; return true; }
        }

        // <NG-RULE>: combines preceeding "n" and incoming key "g"'
        if (k=='g' || k=='G') {
            if (POJLastChar(last)=='N') { syllable[li]+="G"; return true; }
            if (POJLastChar(last)=='n') { syllable[li]+="g"; return true; }
        }        
    }
    
    // <NN-RULE>: if the incoming "n" is preceeded by another "n" AND 
    // a vowel, combines the two n's into nn (lowercased)
    if ((s >= 2) && (k=='n' || k=='N')) {
        int li=s-1;
        string last_one=syllable[li];
        string last_two=syllable[li-1];
        
        if (POJIsCharInList(POJLastChar(last_two), POJ_VOWEL) &&
            POJIsCharInList(POJLastChar(last_one), "nN"))
        {
            syllable[li]="nn";
            return true;
        }
    }
    
    string x(1, k);
    syllable.push_back(x);
 
    return true;
}


void POJSyllable_Holo::backspace() {
    if (syllable.size()) {
        syllable.pop_back();
    }
}

void POJSyllable_Holo::normalize() {
}

const string POJSyllable_Holo::composite() {
    string s;
    for (size_t i=0; i<syllable.size(); i++) {
        s+=syllable[i];
    }
    return s;
}

void msg(const char *s) {
    cout << s << endl;
}

void test(const char *m, POJSyllable_Holo& h, const char *ks) {
    cout << "test case " << m << ", test string=" << ks << endl;
    h.clear();
    for (size_t i=0; i<strlen(ks); i++) {
        h.add(ks[i]);
    }
    h.dump();
}

int main() {
    POJSyllable_Holo h;
   
    msg("test case 1");
    h.clear();
    h.add('p');
    h.add('a');
    h.add('h');
    h.dump();
    
    msg("test case 2");
    h.clear();
    h.add('b');
    h.add('o');
    h.dump();
    
    msg("test case 3");
    h.clear();
    h.add('b');
    h.add('o');
    h.add('u');
    h.dump();
    
    msg("test case 4");
    h.clear();
    h.add('B');
    h.add('O');
    h.add('U');
    h.dump();
    
    msg("test case 5");
    h.clear();
    h.add('b');
    h.add('o');
    h.add('U');
    h.dump();
    
    msg("test case 6");
    h.clear();
    h.add('B');
    h.add('O');
    h.add('u');
    h.dump();

    msg("test case 7: Ng");
    h.clear();
    h.add('N');
    h.add('g');
    h.dump();

    msg("test case 8: NNg");
    h.clear();
    h.add('N');
    h.add('N');
    h.add('g');
    h.dump();

    msg("test case 9: aNn");
    h.clear();
    h.add('a');
    h.add('N');
    h.add('n');
    h.dump();

    test("test case 10: bNn", h, "bNn");

    test("test case 11: aiounN", h, "aiounN");

    test("test case 12: Nn", h, "Nn");
    test("test case 13: nnG", h, "nnG");
    
    test("14", h, "^ainn");
    test("15", h, "a^inn");
    test("16", h, "|ainn");
    test("17", h, "|ainn");
    test("18", h, "^ouinn");
    test("19", h, "|ainn");
    test("20", h, "|ainh");
    test("21", h, "p^ah");
    test("22", h, "|ainh");
    test("23", h, "^nng");
    test("24", h, "n=ng");
    test("25", h, "`nng");
    test("26", h, "h^ounn");
    test("27", h, "h=ounng");
    
 	return 0;
}


/*

 POJ Syllables:
    


 現在狀況是，剛剛把初步的 POJ syllable grammar 寫出來
 準備要展開成一個一個的 state 了

 see://vp081180.reshsg.uci.edu/

 BNF for POJ-Holo (non-standard BNF with regex connotations)
 
 <POJ>      := <H-RULE> | <M-RULE> | <N-RULE> | <CVC-RULE>
 <H-RULE>   := h <VC-RULE> | hm | hmh | hng        # tone on m in hm*, ng on hng
 <M-RULE>   := m <VC-RULE> | mh                    # tone on m in mh
 <N-RULE>   := n <VC-RULE> | ng | nng              # tone on ng in ng and nng
 
 <CVC-RULE> := <SIANN-BU>? <VC-RULE>
 <SIANN-BU> := any of {b, l, p, ph, t, th, k, kh, ch, chh, s}
               # m, n in m/n-rules
 <VC-RULE>  := <VOWEL> <ENDING>?

                               '     '  '  '  '   '  '  ' '  '   '   '
 <VOWEL>    := any of {a,e,i,o,ou,u,ia,io,iou,iu,oa,oe,ui,ai,au,oai,iau}
               # we must allow tone override here

 <ENDING>   := any of {h, nn, m, n, ng, p, t, k}
 

  PC 原來的筆記

  # These are part of the normalization process, which should be an option
  # ie. if user turns off normalization, s/he can produce whatever s/he wants
  
  # vowel toning rules (precedence of toned vowel)
  # a>ou>e>o>i>u
  
  # toning rules
  # {hm, hng, ng, nng} -> 1,2,3,5,7
  # {hmh} -> 4, 8
  # 
  # <VOWEL>,nn,m,n,ng -> 1,2,3,5,7  # <VOWEL> + any of {h,p,t,k} -> 4,8
  # if none -> tone 1

  # simplification rule
  # ioun -> ion
  
  # nasalization rule: nn omitted as vowel is always nasalized in this case
  # {m|n|ng} + vowel + nn -> {m|n|ng} + vowee
  # POJ={siann-bu}+{un-bu}+{un-be}, {un-be}={{open un-be},{closed un-be}}# POJ={{},p,ph,m,t,th,n,k,kh,ng,h,ch,chh,s}+{ng}+{{},h}# POJ={{},h}+{m}+{{},h}# {siann-bu}={{},p,ph,m,b,t,th,n,l,k,kh,ng,g,h,ch,chh,j,s}# {un-bu}={a,e,i,o,ou,u,ia,io,iou,iu,oa,oe,ui,ai,au,oai,iau}# {open un-be}={{},h,nn}# {closed un-be}={m,n,ng,p,t,k}
# {m,n,ng}+{un-bu}+{nn} ==> {m,ng,ng}+{un-bu}+{{}}# {ou}+{closed un-be} ==> {o}+{closed un-be}# {ou}+{open un-be}  !==> {o}+{open un-be}

  # {iou}+{closed un-be} ==> {io}+{closed un-be}# {iou}+{open un-be} ==> illegal
*/


/*
    string x;
    string x("hello");
    string x=y;
    x=y;
    x=y+z;
    x=y+"another";
    size_t len=x.length();
    const char *cstr=x.c_str();
    system(x.c_str());
    
    
    vector<string> v;           // array
    v.push_back(element);
    size_t size=v.size();
    string x=v[0];
    if (v.empty())  { }
    v.clear();
    
    
*/

const char *pojHoloToneComposeKey=" 12345678";
const char *pojHoloVowel="aeimnoquAEIMNOQU";
const char *pojHoloNasel="\xe2\x81\xbf";
// USE U+0358 (DEFAULT)
const char *pojHoloToneTable[]= {
    "a", "\xc3\xa1", "\xc3\xa0", "a", "\xc3\xa2", "\xc3\xa1", "\xc4\x81", "a\xcc\x8d",
    "e", "\xc3\xa9", "\xc3\xa8", "e", "\xc3\xaa", "\xc3\xa9", "\xc4\x93", "e\xcc\x8d",
    "i", "\xc3\xad", "\xc3\xac", "i", "\xc3\xae", "\xc3\xad", "\xc4\xab", "i\xcc\x8d",
    "m", "\xe1\xb8\xbf", "m\xcc\x80", "m", "m\xcc\x82", "\xe1\xb8\xbf", "m\xcc\x84", "m\xcc\x8d",
    "n", "\xc5\x84", "\xc7\xb9", "n", "n\xcc\x82", "\xc5\x84", "n\xcc\x84", "n\xcc\x8d",
    "o", "\xc3\xb3", "\xc3\xb2", "o", "\xc3\xb4", "\xc3\xb3", "\xc5\x8d", "o\xcc\x8d",
    "o\xcd\x98","\xc3\xb3\xcd\x98","\xc3\xb2\xcd\x98","o\xcd\x98","\xc3\xb4\xcd\x98","o\xcd\x98","\xc5\x8d\xcd\x98","o\xcc\x8d\xcd\x98",
    "u", "\xc3\xba", "\xc3\xb9", "u", "\xc3\xbb", "\xc3\xba", "\xc5\xab", "u\xcc\x8d",
    "A", "\xc3\x81", "\xc3\x80", "a", "\xc3\x82", "\xc3\x81", "\xc4\x80", "A\xcc\x8d",
    "E", "\xc3\x89", "\xc3\x88", "E", "\xc3\x8a", "\xc3\x89", "\xc4\x92", "E\xcc\x8d",
    "I", "\xc3\x8d", "\xc3\x8c", "I", "\xc3\x8e", "\xc3\x8d", "\xc4\xaa", "I\xcc\x8d",
    "M", "\xe1\xb8\xbe", "M\xcc\x80", "M", "M\xcc\x82", "\xe1\xb8\xbe", "M\xcc\x84", "M\xcc\x8d",
    "N", "\xc5\x83", "\xc7\xb8", "N", "N\xcc\x82", "\xc5\x83", "N\xcc\x84", "N\xcc\x8d",
    "O", "\xc3\x93", "\xc3\x92", "O", "\xc3\x94", "\xc3\x93", "\xc5\x8c", "O\xcc\x8d",
    "O\xcd\x98","\xc3\x93\xcd\x98","\xc3\x92\xcd\x98","O\xcd\x98","\xc3\x94\xcd\x98","\xc3\x93\xcd\x98","\xc5\x8c\xcd\x98","O\xcc\x8d\xcd\x98",
    "U", "\xc3\x9a", "\xc3\x99", "U", "\xc3\x9b", "\xc3\x9a", "\xc5\xaa", "U\xcc\x8d"
};
// USE MIDDLE DOT (OPTIONAL, FALLBACK)
const char *pojHoloToneTableMiddleDot[]= {
    "a", "\xc3\xa1", "\xc3\xa0", "a", "\xc3\xa2", "\xc3\xa1", "\xc4\x81", "a\xcc\x8d",
    "e", "\xc3\xa9", "\xc3\xa8", "e", "\xc3\xaa", "\xc3\xa9", "\xc4\x93", "e\xcc\x8d",
    "i", "\xc3\xad", "\xc3\xac", "i", "\xc3\xae", "\xc3\xad", "\xc4\xab", "i\xcc\x8d",
    "m", "\xe1\xb8\xbf", "m\xcc\x80", "m", "m\xcc\x82", "\xe1\xb8\xbf", "m\xcc\x84", "m\xcc\x8d",
    "n", "\xc5\x84", "\xc7\xb9", "n", "n\xcc\x82", "\xc5\x84", "n\xcc\x84", "n\xcc\x8d",
    "o", "\xc3\xb3", "\xc3\xb2", "o", "\xc3\xb4", "\xc3\xb3", "\xc5\x8d", "o\xcc\x8d",
    "o\xc2\xb7","\xc3\xb3\xc2\xb7","\xc3\xb2\xc2\xb7","o\xc2\xb7","\xc3\xb4\xc2\xb7","o\xc2\xb7","\xc5\x8d\xc2\xb7","o\xcc\x8d\xc2\xb7",
    "u", "\xc3\xba", "\xc3\xb9", "u", "\xc3\xbb", "\xc3\xba", "\xc5\xab", "u\xcc\x8d",
    "A", "\xc3\x81", "\xc3\x80", "a", "\xc3\x82", "\xc3\x81", "\xc4\x80", "A\xcc\x8d",
    "E", "\xc3\x89", "\xc3\x88", "E", "\xc3\x8a", "\xc3\x89", "\xc4\x92", "E\xcc\x8d",
    "I", "\xc3\x8d", "\xc3\x8c", "I", "\xc3\x8e", "\xc3\x8d", "\xc4\xaa", "I\xcc\x8d",
    "M", "\xe1\xb8\xbe", "M\xcc\x80", "M", "M\xcc\x82", "\xe1\xb8\xbe", "M\xcc\x84", "M\xcc\x8d",
    "N", "\xc5\x83", "\xc7\xb8", "N", "N\xcc\x82", "\xc5\x83", "N\xcc\x84", "N\xcc\x8d",
    "O", "\xc3\x93", "\xc3\x92", "O", "\xc3\x94", "\xc3\x93", "\xc5\x8c", "O\xcc\x8d",
    "O\xc2\xb7","\xc3\x93\xc2\xb7","\xc3\x92\xc2\xb7","O\xc2\xb7","\xc3\x94\xc2\xb7","\xc3\x93\xc2\xb7","\xc5\x8c\xc2\xb7","O\xcc\x8d\xc2\xb7",
    "U", "\xc3\x9a", "\xc3\x99", "U", "\xc3\x9b", "\xc3\x9a", "\xc5\xaa", "U\xcc\x8d"
};




