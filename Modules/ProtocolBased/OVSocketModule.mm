#define OV_DEBUG
#import <Cocoa/Cocoa.h>
#import <OpenVanilla/OpenVanilla.h>
#import <OpenVanilla/OVLibrary.h>
#import <OpenVanilla/OVUtility.h>
#import <unistd.h>
#import "NSStringExtension.h"

enum {
    ovbufclear=1,
    ovbufappend=2,
    ovbufsend=3,
    ovbufupdate=4,
    ovbufupdatewithcursor=5,
    ovcandiclear=10,
    ovcandiappend=11,
    ovcandihide=12,
    ovcandishow=13,
    ovcandiupdate=14,
    ovsrvnotify=20,
    ovkeyreject=30,
    ovkeyaccept=31
};

char serverScript[256];

class OVSocket : public OVBase {
public:
    void open() {
        [NSStream getStreamsToHost: [NSHost hostWithName: @"localhost"] port:20310 inputStream:&is outputStream:&os];
        [is open];
        [os open];
    }
    
    void close() {
        [is close];
        [os close];
    }
    
    void loadAndOpen(OVService *srv) {
        open();
        writeString(@"ping\n");
        if (!readString()) {
            srv->notify("server not loaded, loading");
            system(serverScript);
            sleep(1);
            srv->notify("server loaded");            
            open();
        }
    }

    void writeString(NSString *s) {
        const char *u8s=[s UTF8String];
        [os write:(const uint8_t*)u8s maxLength:strlen(u8s)];
    }

    NSArray *magic(OVService *srv, NSString *s) {
        loadAndOpen(srv);
        writeString(s);
        NSString *r=readString();
        if (!r) {
            srv->notify("server loading failed!");
            return nil;
        }
        close();
        return [r splitBySpaceWithQuote];
    }

    NSString *readString() {
        uint8_t buf[8192];
        int l=[is read:buf maxLength:8192];
        if (l==-1) return nil;
        buf[l]=0;
        return [[NSString stringWithUTF8String:(const char*)buf] stringByChomping];
    }

protected:
    NSInputStream* is;
    NSOutputStream* os;
};

class OVSocketIMContext : public OVInputMethodContext {
public:
    OVSocketIMContext(NSDictionary *ad) {
        atomdict=ad;
        data=[[NSMutableString alloc] initWithString:@"0"];
    }
    ~OVSocketIMContext() { [data release]; }
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {}
    virtual void clear() {
        [data setString:@"0"];
    }
    virtual void end() {}
    virtual int keyEvent(OVKeyCode* key, OVBuffer* buf, OVCandidate* candi, OVService* srv) {
        OVSocket sk;
        NSArray *a=sk.magic(srv, 
            [NSString stringWithFormat: @"OVIMPerlTest %@ %d %d %d\n",
                data, key->code(), buf->isEmpty(), candi->onScreen()
            ]);
        NSLog([a description]);
        if (!a) return 0;
            
        NSEnumerator *e=[a objectEnumerator];
        [data setString: [e nextObject]];
        NSString *s;
        while (s=[e nextObject]) {
            NSNumber *n=[atomdict valueForKey:s];
            if (!n) continue;
            
            switch ([n intValue]) {
                case ovbufclear:
                    buf->clear();
                    break;
                case ovbufappend:
                    buf->append([[e nextObject] UTF8String]);
                    break;
                case ovbufsend:
                    buf->send();
                    break;
                case ovbufupdate:
                    buf->update();
                    break;
                case ovbufupdatewithcursor:
                    buf->update();  // not implemented
                    break;
                case ovcandiclear:
                    candi->clear();
                    break;
                case ovcandiappend:
                    candi->append([[e nextObject] UTF8String]);
                    break;
                case ovcandihide:
                    candi->hide();
                    break;
                case ovcandishow:
                    candi->show();
                    break;
                case ovcandiupdate:
                    candi->update();
                    break;
                case ovsrvnotify:
                    srv->notify([[e nextObject] UTF8String]);
                    break;
                case ovkeyreject:
                    return 0;
                case ovkeyaccept:
                    return 1;
            }
        }
    
        return 0;
    }
    
protected:
    NSDictionary *atomdict;
    NSMutableString *data;
};

class OVSocketInputMethod : public OVInputMethod {
public:
    OVSocketInputMethod() {
        atomdict=[NSMutableDictionary new];        
        [atomdict setValue:[NSNumber numberWithInt:ovbufclear           ]   forKey:@"bufclear"];
        [atomdict setValue:[NSNumber numberWithInt:ovbufappend          ]   forKey:@"bufappend"];
        [atomdict setValue:[NSNumber numberWithInt:ovbufsend            ]   forKey:@"bufsend"];
        [atomdict setValue:[NSNumber numberWithInt:ovbufupdate          ]   forKey:@"bufupdate"];
        [atomdict setValue:[NSNumber numberWithInt:ovbufupdatewithcursor]   forKey:@"bufupdatewithcursor"];
        [atomdict setValue:[NSNumber numberWithInt:ovcandiclear         ]   forKey:@"candiclear"];
        [atomdict setValue:[NSNumber numberWithInt:ovcandiappend        ]   forKey:@"candiappend"];
        [atomdict setValue:[NSNumber numberWithInt:ovcandihide          ]   forKey:@"candihide"];
        [atomdict setValue:[NSNumber numberWithInt:ovcandishow          ]   forKey:@"candishow"];
        [atomdict setValue:[NSNumber numberWithInt:ovcandiupdate        ]   forKey:@"candiupdate"];
        [atomdict setValue:[NSNumber numberWithInt:ovsrvnotify          ]   forKey:@"srvnotify"];
        [atomdict setValue:[NSNumber numberWithInt:ovkeyreject          ]   forKey:@"keyreject"];
        [atomdict setValue:[NSNumber numberWithInt:ovkeyaccept          ]   forKey:@"keyaccept"];        
    }
    
    virtual const char *identifier() { return "OVSocketInputMethod"; }
    virtual OVInputMethodContext *newContext() { return new OVSocketIMContext(atomdict); }
    
protected:
    NSMutableDictionary *atomdict;
};
    
class OVSocketOutputFilter : public OVOutputFilter {
public:
    OVSocketOutputFilter() { data=[[NSMutableString alloc] initWithString:@"0"]; }
    ~OVSocketOutputFilter() { [data release]; }    
    virtual const char *identifier() { return "OVSocketOutputFilter"; }
    virtual const char *process(const char *src, OVService *srv) {
        OVSocket s;
        NSArray *a=s.magic(srv, [NSString stringWithFormat:@"OVOFPerlTest %@ %@\n", data, [NSString stringWithUTF8String:src]]);
        if (!a) return "";
        NSLog([a description]);
        [data setString:[a objectAtIndex:0]];
        return [[a objectAtIndex:1] UTF8String];
    }
protected:
    NSMutableString *data;
};

extern "C" unsigned int OVGetLibraryVersion() { return OV_VERSION; }
extern "C" int OVInitializeLibrary(OVService*, const char*) {
    NSApplicationLoad();
    [NSAutoreleasePool new];
    sprintf(serverScript, "/Users/lukhnos/logiciels/openvanilla/Modules/ProtocolBased/OVSocketServer.pl &");
    return 1;
}
extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
    switch (idx) {
        case 0: return new OVSocketOutputFilter;
        case 1: return new OVSocketInputMethod;
    }
    return NULL;
}
