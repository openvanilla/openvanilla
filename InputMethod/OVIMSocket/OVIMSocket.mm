#define OVDEBUG
#import <Cocoa/Cocoa.h>
#import <OpenVanilla/OpenVanilla.h>
#import <OpenVanilla/OVLoadable.h>
#import <OpenVanilla/OVUtility.h>

class OVIMSocket;

class OVIMSocketContext : public OVIMContext
{
public:
    OVIMSocketContext(OVIMSocket *p) : parent(p), is(NULL), os(NULL) {}
    
    virtual int keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar, 
        OVService* srv);
        
    virtual int activate(OVService*)
    {
        [NSStream getStreamsToHost: [NSHost hostWithName: @"localhost"] 
            port: 7986 inputStream: &is outputStream: &os];
        [is retain];
        [os retain];
        [is open];
        [os open];
        return 1;
    }
    
    virtual int deactivate(OVService*)
    {
        [is close];
        [os close];
        return 1;
    }
    
protected:
    int writeBuf(char *buf)
    {
        int l=[os write: buf maxLength: strlen(buf)];
        murmur ("OVIMSocket: wrote %d bytes to socket server", l);
        return l;
    }
    
    int readBuf(char *buf)
    {
        int l=[is read: buf maxLength: 255];
        murmur ("OVIMSocket: read %d bytes from socket server", l);
        return ;
    }

    OVIMSocket *parent;
    NSInputStream* is;
    NSOutputStream* os;
};

class OVIMSocket : public OVInputMethod
{
public:
    virtual int identifier(char *s) { return strlen(strcpy(s, "OVIMSocket")); }
    
    virtual int name(char *locale, void *s, OVEncoding *enc)
    {
        *enc=ovEncodingUTF8;
        return strlen(strcpy((char*)s, "OV Socket-Perl"));
    }
    
    virtual OVIMContext *newContext() { return new OVIMSocketContext(this); }
};

int OVIMSocketContext::keyEvent(OVKeyCode *key, OVBuffer *buf, OVTextBar *textbar,
    OVService* srv)
{
    char sbuf[256];
    char skey[256], value[256];
    
    if (buf->length() && key->code()==ovkReturn)
    {
        buf->send();
        return 1;
    }
    
    if (key->isPrintable())
    {
        sprintf(sbuf, "key %c\n", key->code());
        writeBuf(sbuf);
        
        while (!readBuf(sbuf)) ;
        sscanf(sbuf, "%s %s", skey, value);
        murmur ("OVIMSocket: server response! key=%s, value=%s", skey, value);
        if (!strcasecmp(skey, "buffer-add"))
        {
            buf->append(value)->update();
            return 1;
        }
    }
    
    return 0;
}


OVLOADABLEWRAPPER(OVIMSocket);
OVLOADABLEOBJCWRAPPER;
