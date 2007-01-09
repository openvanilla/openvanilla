#include "AVConfig.h"
#include <stdio.h>

int main()
{
    AVConfig cfg("./org.openvanilla.072.plist");
    
    if (!cfg.load()) {
        printf ("failed to load\n");
    }
    
    
    if (cfg.dictionary().dictionaryExists("TLIM")) {
        AVDictionary tlim = cfg.dictionary().getDictionary("TLIM");
        int d = tlim.getInteger("diacriticInputOption");
        printf ("original value = %d\n", d);
    }

    fgetc(stdin);
    
    if (cfg.needSync()) {
        printf ("need to sync!");
        cfg.load();
    }
    
    AVDictionary dict = cfg.dictionary();
    
    if (dict.dictionaryExists("TLIM")) {
        AVDictionary tlim = dict.getDictionary("TLIM");
        
        int d = tlim.getInteger("diacriticInputOption");
        printf ("value = %d\n", d);
        d++;
        tlim.setInteger("diacriticInputOption", d);
    }
    
    dict.createDictionaryForKey("Blah");
    
    if (!cfg.write()) {
        printf ("failed to write\n");
    }
}

/*

int old_main() {
    FILE *f = fopen("./org.openvanilla.072.plist", "r");
    
    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    
    char *buf = (char*)calloc(1, size);
    fread(buf, size, 1, f);
    fclose(f);
        
    PlistParser parser;
    PlistNode &parent = parser.parse(buf, size); 
    free(buf);
    
    // first node
    PlistNode *plist = parent.childAtIndex(0);
    if (!plist) abort();
    
    PlistNode *dict = plist->childAtIndex(0);
    if (!dict) abort();
    
    PlistNode *ovds = dict->findChildByKey(string("OVDisplayServer"));
    
    cout << ovds->str(0, string());
    
    cout << "===" << endl;
    
    PlistNode *bi = ovds->findChildByKey(string("backgroundImage"));
    cout << bi->str(0, string()) << endl;
    cout << "===" << endl;
    
    PlistNode attr(string("string"), ovds);
    attr.setText(string("foobar"));
    ovds->setChildByKey(string("backgroundImage"), attr);

    cout << ovds->str(0, string());
    

    cout << "####\n";
    
    AVDictionary ro(new PlistNode(*ovds), true);
    AVDictionary wr(ovds, false);
    
    cout << "background of wr: " << wr.getString("background") << endl;
    cout << "background of ro: " << ro.getString("background") << endl;
    
    wr.setString("background", "abcdefg");
    wr.createDictionaryForKey("FOOOOOOfoobar");
    wr.setString("blah1234", "231123123213");
    wr.setInteger("12345678", 5678);
    
    ro.setString("background", "1234567");
    ro.createDictionaryForKey("123212131231231212");
    ro.setInteger("fooooo", 123);
    ro.setString("abcd", "12213123123123121231");
    
    cout << "ovds: ######\n";
    
    cout << ovds->str(0, string());
    
    cout << "wr: ######\n";
    cout << wr.dataNode()->str(0);
    cout << "ro: ######\n";
    
    cout << ro.dataNode()->str(0);
    
    
    
    string xmlhead = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
    
    cout << "finally #########\n";
    cout << parent.str(0, xmlhead);
}

*/
