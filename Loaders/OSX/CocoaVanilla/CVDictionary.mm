// CVDictionary.mm

#import "CVDictionary.h"

CVDictionary::CVDictionary(NSMutableDictionary *dict) {
    d=dict;
    [d retain];
}

CVDictionary::~CVDictionary() {
    [d release];
}

int CVDictionary::keyExist(const char *key) {
    return [d objectForKey:[NSString stringWithUTF8String:key]] ? 1 : 0;
}

int CVDictionary::getInteger(const char *key) {
    NSString *s;
    return (s=[d objectForKey: [NSString stringWithUTF8String:key]]) ? [s intValue] : 0;
}

int CVDictionary::setInteger(const char *key, int value) {
    [d setObject: [NSString stringWithFormat: @"%d", value] forKey: [NSString stringWithUTF8String:key]];
    return value;
}

const char* CVDictionary::getString(const char *key) {
    NSString *s;
    return (s=[d objectForKey: [NSString stringWithUTF8String:key]]) ? [s UTF8String] : "";
}

const char* CVDictionary::setString(const char *key, const char *value) {
    [d setObject:[NSString stringWithUTF8String:value] forKey:[NSString stringWithUTF8String:key]];
    return value;
}