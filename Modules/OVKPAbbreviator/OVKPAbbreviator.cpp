// OVKPAbbreviator.cpp
// An OpenVanilla automatic key abbreviator.

// [author=b6s]
// [license]

#include "OVKPAbbreviator.h"

const char* OVKPAbbreviator::identifier() { return "OVKPAbbreviator"; }
const char* OVKPAbbreviator::localizedName(const char* locale) {
	if (!strcasecmp(locale, "zh_TW") || !strcasecmp(locale, "zh_CN"))
		return "自動縮寫器";
	return "Abbreviator";
}

OVInputMethodContext* OVKPAbbreviator::newContext() {
    return new OVKPAbbreviatorContext(this);
}

int OVKPAbbreviatorContext::keyEvent(OVKeyCode* k, OVBuffer* b, OVCandidate* i, OVService* s) 
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
        s->notify("you're in abbreviator mode");
        return 1;
    }
    
    if (c == ovkReturn) {
		const char *c=seq.content();
		murmur ("OVKPAbbreviator: executing command \"%s\"", c);

		// search for abbreviations
		if (strlen(c) > 2 && c[0]=='`' && c[1]==' ') {
			b->clear()->update();                 
			SQLite3Statement *st =
				parent->getDb()->prepare(
					"select value from abbreviation where key='%q';", c);
			if (st) {
				if (st->step() == SQLITE_ROW) {
					s->notify("Abbreviation found and committed");
					b->append(st->column_text(0))->send();
				}
				else
					s->notify("Abbreviation not found");
			}
			else
				s->notify("SQLite error");

			delete st;
			clear();

			return 1;
		}
			
		// log sentences
		int r;
		time_t t;
		time(&t);
		r=parent->getDb()->execute(
			"insert into abbreviation values('%q', '%q', '%d');",
			seq.content(), parent->getLast(), t);
		murmur("OVKPAbbreviator: sqlite3 returns %d, errmsg=%s",
			r, parent->getDb()->errmsg());

		//b->clear()->update();
		clear();
                
		const char *mt="new abbreviation added, key=%s, value=%s";
		char *msg=(char*)calloc(1, strlen(c+2)+strlen(mt)+strlen(parent->getLast())+1);
		sprintf(msg, mt, c+2, parent->getLast());
		s->notify(msg);
		free(msg);
		return 1;
    }
    
    if (!isprint(c)) {
        s->beep();
        return 1;
    }
    
    seq.add(c);
    //b->clear()->append(seq.content())->update();
    working=true;
    s->notify("you're in abbreviator mode");
    
    return 1;
}

const char* OVOFAbbreviationCatcher::process (const char *src, OVService *srv) {
        const char *p="caught abbreviation: ";
        char *b=(char*)calloc(1, strlen(p)+strlen(src)+1);
        strcpy(b, p);
        strcat(b, src);
        if (display) srv->notify(b);
        free(b);
        
        if (last_) free(last_);
        last_=(char*)calloc(1, strlen(src)+1);
        strcpy(last_, src);
        
        return src;
}