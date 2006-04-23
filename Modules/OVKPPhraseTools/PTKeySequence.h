// OVKPKeySequence.h: Key sequence class for PhraseTools

#ifndef __PTKeySequence_h
#define __PTKeySequence_h

#include <string>
#include <vector>

using namespace std;

class PTKeySequence {
public:
    void clear() { list.clear(); }
    void add(const string &x) { list.push_back(string(x)); }
    void pop() { if (list.size()) list.pop_back(); }
    int count() { return list.size(); }
    const string compose() {
        string c;
        for (size_t i=0; i<list.size(); i++) c+=list[i];
        return c;
    }

protected:
    vector<string> list;    
};

#endif

