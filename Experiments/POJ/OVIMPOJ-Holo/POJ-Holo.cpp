#include <string>
#include <map>
#include <vector>
#include <stdio.h>

using namespace std;

void foo() {
	printf ("hello, world!\n");
}

int main() {
    string x;
    map<string, int> m;
    m[string("hello")]=1;
    m[string("world")]=2;
    
    printf ("%d\n", m["hello"]);
	foo();
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


