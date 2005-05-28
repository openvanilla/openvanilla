// POJ-Holo.h: Definition of POJ-Holo's key sequence

#ifndef __POJHolo_h
#define __POJHolo_h

// keyboard layouts
enum {
    POJ_Holo_ToneByNumber=0,
    POJ_Holo_ToneByPreceedingSymbol=1,
    POJ_Holo_ToneBySucceedingSymbol=2
};

const int POJ_Holo_MaxSeqLen=16, POJ_Holo_MaxBufLen=64;

class POJHoloKeySequence
{
public:
    POJHoloKeySequence();
    
    bool isEmpty();
    void clear();

    bool isComposeKey(char c);
    bool add(char c, int layout=POJ_Holo_ToneByNumber);
    void remove();

    void normalize();           // normalize before finalize
    const char *sequence();
    const char *finalize();     // return finalized sequence (+ tone)
    const char *compose(bool pureascii=false);

    int toneMark(char c);
    
protected:
    int len;
    char seq[POJ_Holo_MaxSeqLen];
    char composebuf[POJ_Holo_MaxBufLen];

    int presettone;
    int prebindwait;
    int bindvowel;
    
    // service functions
    int vowelorder(char c);
    const char *vowel2tone(char c, int tone);
};

#endif
