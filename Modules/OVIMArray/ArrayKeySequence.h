#ifndef _ARRAYKEYSEQUENCE_H
#define _ARRAYKEYSEQUENCE_H

#include<OpenVanilla/OpenVanilla.h>
#include"OVKeySequence.h"
#include"OVCIN.h"
#include<string>

class ArrayKeySequence : public OVKeySequenceSimple
{
protected:
    OVCIN* cinTable;
public:
    ArrayKeySequence(OVCIN* tab) : cinTable(tab) {}
    virtual int length() { return len; }
    virtual int add(char c){
//        if (valid(c) == 0) return 0;
        return OVKeySequenceSimple::add(c);
    }
    virtual int valid(char c){
        std::string inKey;
        inKey.push_back(c);
        if (!cinTable->isValidKey(inKey)) return 0;
        return 1;
    
    }
    virtual std::string& compose(std::string& s){
        for (int i=0; i<len; i++)
        {
            std::string inKey;
            inKey.push_back(seq[i]);
            vector<std::string> outStringVectorRef;
            if (cinTable->getCharVectorByKey(inKey, outStringVectorRef) > 0)
                s.append(outStringVectorRef[0]);
        }
        return s;
    }
    virtual char* getSeq() { return seq; }
    
};

#endif
