// plainvanilla.h

#ifndef __plainvanilla_h
#define __plainvanilla_h

typedef unsigned short PVUTF32;

class PVClass
{
public:
    virtual ~PVClass() {}
};


class PVCandidateIterator : public PVClass
{
public:
    virtual int size() { return 0; }
    
    virtual int move(int) { return 0; } 
    virtual int getlen(int p=0)     { return 0; }
    virtual int get(PVUTF32*, int p=0)    { return 0; }
};

enum
{
    pvbLayoutEten=0,
    pvbLayoutStandard=1
};

class PVCodeSequence : public PVClass
{
public:
    virtual void clear();
    virtual int addkeychar(char) { return 0; }
    virtual int removelast() { clear(); return 0; }
    
    virtual int utfstrlen() { return 0; }
    virtual int copyutfstr(PVUTF32*) { return 0; }
    
    virtual PVCandidateIterator *search() { return 0; }
};


typedef unsigned short VPSymbol;
class PVBpmfIterator;

class PVBpmfBlock : public PVCodeSequence
{
public:
    PVBpmfBlock();
    ~PVBpmfBlock();
    
    void clear();
    int setlayout(int);
    
    int addkeychar(char);
    int removelast();

    int utfstrlen() { return 0; }
    int copyutfstr(PVUTF32*) { return 0; }

    PVCandidateIterator *search();
    
protected:
    PVBpmfIterator *iter;
    VPSymbol sym;
};

const ist pvcMaxSequenceLen=5;
class PVCangjeiIterator;

class PVCangjeiSequence : public PVCodeSequence
{
public:
    PVCangjeiSequence();
    ~PVCangjeiSequence();
    
    void clear();
    int addkeychar(int);
    int removelast();

    int utfstrlen() { return 0; }
    int copyutfstr(PVUTF32*) { return 0; }

    PVCandidateIterator *search();

protected:
    PVCangjeiIterator *iter;
    char seq[pvcMaxSequenceLen];
    int seqlen;
};

#endif  // of #ifndef __plainvanilla_h