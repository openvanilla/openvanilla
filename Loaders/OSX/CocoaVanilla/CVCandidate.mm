// CVCandidate.mm

#include "CVCandidate.h"
#include "OVDisplayServer.h"

CVCandidateState::CVCandidateState(NSString *s, Point p, BOOL o) {
    str=[[NSString alloc] initWithString:s];
    pos=p;
    onscreen=o;
}

CVCandidateState::~CVCandidateState() {
	[str release];
}

CVCandidate::CVCandidate(id s) {
	srvr=s;
	text=[NSMutableString new];
	pos=(Point){0, 0};
	onscreen=NO;
}

CVCandidate::~CVCandidate() {
	[text release];
}

OVCandidate* CVCandidate::clear() {
	[text setString:@""];
    return this;
}

OVCandidate* CVCandidate::append(const char *s) {
    NSString *u8s=[NSString stringWithUTF8String:s];
	if (u8s) [text appendString:u8s];
    return this;
}

OVCandidate* CVCandidate::hide() {
	if (onscreen) {
		[srvr candidateHide];
		onscreen=NO;
	}
    return this;
}

OVCandidate* CVCandidate::show() {
	if (!onscreen) {
		[srvr candidateShow];
		onscreen=YES;
	}
    return this;
}

OVCandidate* CVCandidate::update() {
	[srvr candidateUpdate:text position:pos];
    return this;
}

int CVCandidate::onScreen() {
    return onscreen;
}

OVCandidate* CVCandidate::setPosition(Point p) {
	pos=p;
    return this;
}

CVCandidateState* CVCandidate::saveState() {
	return new CVCandidateState(text, pos, onscreen);
}

OVCandidate* CVCandidate::restoreState(CVCandidateState *s) {
	[text setString:s->str];
	pos=s->pos;
	update();
	if (s->onscreen) show(); else hide();
    return this;
}

