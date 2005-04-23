// CVBuffer.mm

#include "CVBuffer.h"
#include "CVWrappers.h"

CVBuffer::CVBuffer(TSComposingBuffer *cb, NSArray *ofa, OVService *s) {
    cbuf=cb;
    ofarray=ofa;
    srv=s;
}

OVBuffer* CVBuffer::clear() {
    if (cbuf) cbuf->clear();
    return this;
}

OVBuffer* CVBuffer::append(const char *s) {
    if (cbuf) cbuf->append([NSString stringWithUTF8String:s]);
    return this;
}

OVBuffer* CVBuffer::send() {
    if (!cbuf) return this;
    
    if (![ofarray count]) {
        cbuf->send();
        return this;
    }
    
    NSString *src=cbuf->getContent();
    const char *u8=[src UTF8String];
    NSEnumerator *e=[ofarray objectEnumerator];
    id o;
    while (o=[e nextObject]) {
        OVOutputFilter *filter=(OVOutputFilter*)[o module];
        if (!filter) continue;
        const char *result=filter->process(u8, srv);
        if (result==u8) continue;
        src=[NSString stringWithUTF8String:result];
        u8=[src UTF8String];
    }
    
    if (![src length]) {
        cbuf->clear()->update();
    }
    else {
        cbuf->clear()->append(src);
        cbuf->send();
    }
    return this;
}

OVBuffer* CVBuffer::update() {
    if (cbuf) cbuf->update();
    return this;
}

OVBuffer* CVBuffer::update(int cursorPos, int markFrom, int markTo) {
    if (cbuf) cbuf->update(FALSE, cursorPos, markFrom, markTo);
    return this;
}

int CVBuffer::isEmpty() {
    return cbuf ? cbuf->isEmpty() : 0;
}

Point CVBuffer::getAppCursorPosition() {
    return cbuf->getAppCursorPosition();
}

CVBuffer *CVBuffer::setComposingBuffer(TSComposingBuffer *cb) {
    cbuf=cb;
    return this;
}
