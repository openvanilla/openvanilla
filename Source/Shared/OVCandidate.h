// OVCandidate.h

#ifndef __OVCandidate_h
#define __OVCandidate_h

//#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>
#include <string>
#include <vector>

using namespace std;

class OVCandidate : public OVObject
{
public:
    OVCandidate()
    {
        onduty=0;
    }
    
    void prepare(vector<string> *l, char *skey, OVTextBar *textbar);
    int onDuty() { return onduty; }
    int onePage() { return (count <= perpage); }
    void cancel() { onduty=0; }
    void update(OVTextBar *textbar);
    OVCandidate* pageUp();
    OVCandidate* pageDown();
    string* select(char c);
    
protected:
    int onduty;
    char selkey[32];
    int count;
    int perpage;
    int pos;
    vector<string> *list;
};

#endif
