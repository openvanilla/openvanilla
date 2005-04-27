// OVKeySequence.h

#ifndef __OVKeySequence_h
#define __OVKeySequence_h

class OVKeySequence : public OVBase
{
public:
    virtual int add(char) { return 0; }
    virtual OVKeySequence* remove() { return this; }
    virtual OVKeySequence* clear() { return this; }
};

const int ovMaxSeqSimpLen=32;

class OVKeySequenceSimple : public OVKeySequence
{
public:
    OVKeySequenceSimple(int m=ovMaxSeqSimpLen) : len(0), max(m) { seq[0]=0; }
    virtual int add(char c)
    {
        if (len==max) return 0;
        seq[len++]=tolower(c);
        seq[len]=0;
        return 1;
    }
    
    virtual OVKeySequence* remove()
    {
        if (len) seq[--len]=0;
        return this;
    }
    
    virtual OVKeySequence* clear()
    {
        len=0;
        seq[0]=0;
        return this;
    }
    
protected:    
    int len;
    int max;
    char seq[ovMaxSeqSimpLen];
};

#endif
