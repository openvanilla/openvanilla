#ifndef AVBuffer_h
#define AVBuffer_h
#include "OpenVanilla.h"
#include "AVDisplayServer.h"
#include <string>
#include <vector>
using namespace std;

class AVBuffer : public OVBuffer {
public:
    AVBuffer(vector<OVOutputFilter*> *ovof, OVService *s);
    AVBuffer(AVDisplayServer* svr, vector<OVOutputFilter*> *ovof, OVService *s);
    void setDisplayServer(AVDisplayServer* svr);
    virtual OVBuffer* clear();
    virtual OVBuffer* append(const char *s);
    virtual OVBuffer* send();
    virtual OVBuffer* update();
    virtual OVBuffer* update(int cursorPos, int markFrom, int markTo);
    virtual int isEmpty();

private:
    string bufstr;
    AVDisplayServer *dsvr;
    vector<OVOutputFilter*> *ovof_vector;
    OVService *srv;
};
#endif // AVBuffer_h
