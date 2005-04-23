// CVCandidate.mm

#include "CVCandidate.h"

CVCandidate::CVCandidate(CVInfoBox *ib) {
    infobox=ib;
}

OVCandidate* CVCandidate::clear() {
    [infobox clear];
    return this;
}

OVCandidate* CVCandidate::append(const char *s) {
    [infobox append:[NSString stringWithUTF8String:s]];
    return this;
}

OVCandidate* CVCandidate::hide() {
    [infobox hide];    
    return this;
}

OVCandidate* CVCandidate::show() {
    [infobox show];
    return this;
}

OVCandidate* CVCandidate::update() {
    [infobox update];
    return this;
}

int CVCandidate::onScreen() {
    return [infobox onScreen];
}

OVCandidate* CVCandidate::setPosition(Point p) {
    [infobox setPosition:p];
    return this;
}
int CVCandidate::height() {
    return [infobox height];
}

CVInfoBoxState* CVCandidate::saveState() {
    return [infobox saveState];
}

OVCandidate* CVCandidate::restoreState(CVInfoBoxState *s) {
    [infobox restoreState:s];
    return this;
}

