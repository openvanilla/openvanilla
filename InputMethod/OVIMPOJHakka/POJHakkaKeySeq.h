// POJHakkaKeySeq.h

#ifndef __POJHakkaKeySeq_h
#define __POJHakkaKeySeq_h

// keyboard layouts
enum
{
    pojToneByNumber=0,
    pojTonePrebind=1,
    pojTonePostbind=2  
};

int vowelorder(char c);
const char *vowel2tone(char c, int tone);
int tonemark(char c);

const int pojMaxSeqLen=16;

class POJHakkaKeySeq
{
public:
    POJHakkaKeySeq();
    int isCompose(char c);    
    int add(char c, int layout=pojToneByNumber);    
    void remove();
    
    int length() { return len; }
    char *keybuf() { return seq; }
    void clear();
    
    char *normalize();    
    char *finalize();
    char *compose(char *buf, int asciioutput=0);
    
protected:
    int len;
    char seq[pojMaxSeqLen];
    
    int presettone;
    int prebindwait;
    int bindvowel;
};

#endif
