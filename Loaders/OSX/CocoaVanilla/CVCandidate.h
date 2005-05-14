// CVCandidate.h

#ifndef __CVCandidate_h
#define __CVCandidate_h

#include <Cocoa/Cocoa.h>
#include <OpenVanilla/OpenVanilla.h>

struct CVCandidateState {
    CVCandidateState(NSString *s, Point p, BOOL o);
    ~CVCandidateState();
    NSString *str;
    Point pos;
    BOOL onscreen;
};


class CVCandidate : public OVCandidate  {
public:
    CVCandidate(id s);
	~CVCandidate();
    virtual OVCandidate* clear();
    virtual OVCandidate* append(const char *s);
    virtual OVCandidate* hide();
    virtual OVCandidate* show();
    virtual OVCandidate* update();
    virtual int onScreen();
    
    virtual OVCandidate* setPosition(Point p);
    virtual CVCandidateState* saveState();
    virtual OVCandidate* restoreState(CVCandidateState *s);
protected:
	id srvr;
	NSMutableString *text;
	Point pos;
	BOOL onscreen;
};


#endif
