// OVKPPhrase.cpp
// An OpenVanilla key preprocessor application

// [author=lukhnos]
// [license]

#include "OVKPPhrase.h"
#include "OVKPPInterface.h"
    
const char* OVOFPhraseCatcher::process (const char *src, OVService *srv) {
        const char *p="caught phrase: ";
        char *b=(char*)calloc(1, strlen(p)+strlen(src)+1);
        strcpy(b, p);
        strcat(b, src);
        if (display) srv->notify(b);
        free(b);
        
        if (last) free(last);
        last=(char*)calloc(1, strlen(src)+1);
        strcpy(last, src);
        
        return src;
}    

int OVKPPhraseTool::initialize(OVDictionary *moduleCfg, OVService *srv, const char *modulePath) {
    update(moduleCfg, srv);
    return 1;
}

void OVKPPhraseTool::update(OVDictionary *moduleCfg, OVService *srv) {
    const char *s=moduleCfg->getStringWithDefault("activateKey", "@");
    wk=s[0];
}

int OVKPPhraseToolContext::keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* s) 
{
    char workkey=parent->getActivateKey();
    int c=k->code();

    if (working && !seq.length() && (!isprint(k->code()) || k->isCommand())) {
        clear();
        return 0;
    }

    // repeated work key is sent back    
    if (working && !seq.length() && c==workkey) {
        clear();
        return 0;
    }

    if (!working) {
        // if not workkey, returns
        if ((k->code() != workkey) || k->isCommand()) return 0;
        
        // if something is in buffer (put by some other IM's), returns
        if (!b->isEmpty()) return 0;
        
        // workkey hitted
        working=true;
        s->notify("you're in phrase tool mode");
        return 1;
    }
    
    if (c == ovkBackspace || c == ovkDelete) {
        seq.remove();
        if (!seq.length()) {
            working=false;
            b->clear()->update();
            return 1;
        }
        b->clear()->append(seq.content())->update();
        return 1;
    }
    
    if (c == ovkEsc) {
        clear();
        return 1;
    }
    
    if (c == ovkReturn) {
        if (!seq.length()) {
            clear();
            return 0;
        }
        else {                
            const char *c=seq.content();
            murmur ("OVKPPhrase: executing command \"%s\"", c);

            // append phrase                
            if (strlen(c) > 2 && c[0]=='a' && c[1]==' ') {
                int r;
                time_t t;
                time(&t);
                r=phdb->execute("insert into phrase values('%q', '%q', '%d');", c+2, last, t);
                murmur("OVKPPhrase: sqlite3 returns %d, errmsg=%s", r, phdb->errmsg());

                b->clear()->update();
                clear();
                
                const char *mt="new phrase added, key=%s, value=%s";
                char *msg=(char*)calloc(1, strlen(c+2)+strlen(mt)+strlen(last)+1);
                sprintf(msg, mt, c+2, last);
                s->notify(msg);
                free(msg);
                return 1;
            }

            // otherwise we're in search mode

            b->clear()->update();                                
            SQLite3Statement *st=phdb->prepare("select value from phrase where key='%q';", c);
            if (st) {
                if (st->step() == SQLITE_ROW) {
                    s->notify("Phrase found and committed");
                    b->append(st->column_text(0))->send();
                }
                else {
                    s->notify("Phrase not found");
                }
            }
            else {
                s->notify("SQLite error");
            }
            delete st;
                        
            clear();
            return 1;
        }
    }
    
    if (!isprint(c)) {
        s->beep();
        return 1;
    }
    
    seq.add(c);
    b->clear()->append(seq.content())->update();
    working=true;
    s->notify("you're in phrase tool mode");
    
    return 1;
}