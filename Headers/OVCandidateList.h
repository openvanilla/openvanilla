// OVCandidateList.h

#ifndef __OVCandidateList_h
#define __OVCandidateList_h

//#include <Cocoa/Cocoa.h>
//#include <OpenVanilla/OpenVanilla.h>
#include "OpenVanilla.h"
#include <string>
#include <vector>

using namespace std;

class OVCandidateList : public OVBase
{
public:
    OVCandidateList()
    {
        onduty=0;
    }
    
    void prepare(vector<string>* l, char* skey, OVCandidate *textbar);
    int onDuty() { return onduty; }
    int onePage() { return (count <= perpage); }
    void cancel() { onduty=0; }
    void update(OVCandidate *textbar);
    OVCandidateList* pageUp();
    OVCandidateList* pageDown();
    bool select(char inKey, string& outStringRef);
	const char* getSelKey() { return selkey; }
protected:
    int onduty;
    char selkey[32];
    int count;
    int perpage;
    int pos;
    vector<string>* list;
};

#endif
