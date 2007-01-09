// AVPlist.h: OS X-Style Property List and Its XML Parser

#ifndef __AVPlist_h
#define __AVPlist_h

#include <stdlib.h>
#include <expat.h>

#include <string>
#include <vector>
#include <sstream>


using namespace std;

class PlistNode
{
public:
    PlistNode(const char *n, PlistNode *p=NULL)
    {
        _name = string(n);
        _parent = p;
    }
    
    PlistNode(string n="", PlistNode *p=NULL) 
    {
        _name=n;
        _parent = p;
    }
    
    ~PlistNode() {}
    
    PlistNode(const PlistNode& n)
    {
        _name=n._name;
        _text=n._text;
        children=n.children;
        _parent=n._parent;
    }
    
    const PlistNode& operator=(const PlistNode& n)
    {
        _name=n._name;
        _text=n._text;
        children=n.children;
        _parent=n._parent;
        return *this;
    }
  
    PlistNode* addChild(const PlistNode& n)
    { 
        children.push_back(n);
        children[children.size()-1]._parent = this;
        return &(children[children.size()-1]);
    }
    
    string str(int tab=0, string xmlhead="") {
        stringstream s;
        
        // if it's the parent node, tab-1 so that the first node has no indent
        int t = _parent ? tab: tab-1;
    
        if (children.size()) {
            if (!_parent) { s << xmlhead; } else { spacer(s, t); startTag(s); }
            
            for (int i=0; i<children.size();i++) s << children[i].str(t+1);
            
            if (_parent) { spacer(s, t); s << "</" << _name << ">\n"; }
        }
        else {
            if (!_parent) {
                s << xmlhead;
            }
            else {
                spacer(s, t);
                if (_text.length()) {
                    s << "<" << _name << ">" << _text << "</" << _name << ">\n";
                }
                else {
                    startTag(s);
                }
            }
        }
        
        return s.str();
    }
    
    PlistNode *parent()
    {
        return _parent;
    }

    const string &text()
    {
        return _text;
    }

    void setText(const string &t)
    {
        _text = t;
    }
    
    const string &name()
    {
        return _name;
    }
    
    void setName(const string &n)
    {
        _name = n;
    }
    
    void setParent(PlistNode *p) 
    {
        _parent = p;
    }
    
    void addText(char c)
    {
        _text += string(1, c);
    }

    void addAttr(const char *k, const char *v)
    {
        pair<string, string> p;
        p.first = string(k);
        p.second = string(v);
        attributes.push_back(p);
    }

    PlistNode *findChildByKey(const char *key)
    {
        return findChildByKey(string(key));
    }
    
    PlistNode *findChildByKey(const string& key)
    {
        size_t i = 0;
        for (i=0 ; i < children.size(); i++) {
            if (children[i].name() == "key" && children[i].text() == key)
                break;
        }
        
        if (i+1 < children.size()) return &(children[i+1]);
        return NULL;
    }

    void setChildByKey(const char *key, const PlistNode& n)
    {
        setChildByKey(string(key), n);
    }
    
    void setChildByKey(const string& key, const PlistNode& n)
    {
        PlistNode *v;
        if (v=findChildByKey(key)) {
            *v = n;
        }
        else {
            PlistNode k("key", this);
            k.setText(key);
            addChild(k);
            addChild(n);
        }
    }
    
    PlistNode *childAtIndex(size_t i)
    {
        if (i >= children.size()) return NULL;
        return &(children[i]);
    }
    
    
protected:
    void startTag(stringstream &s)
    {
        s << "<" << _name;
        
        if (attributes.size()) {
            for (int i=0; i<attributes.size(); i++) {
                s << " " << attributes[i].first << "=\"" << 
                    attributes[i].second << "\"";
            }
        }

        if (!_text.length() && !children.size()) s << "/";
        s << ">\n";
    }
    void spacer(stringstream &s, int tab)
    {
        for (int i=0; i<tab; i++) s << "\t";
    }

    string _name;
    string _text;
    vector<PlistNode> children;
    vector<pair<string, string> > attributes;
    PlistNode *_parent;  
};

class PlistParser {
public:    
    // silently ignores every error
    PlistNode& parse(const char *buf, int size=-1)
    {        
        int s = (size < 0) ? strlen(buf) : size;
        parent = PlistNode();
        current = &parent;

        XML_Parser parser = XML_ParserCreate("UTF-8");
        XML_SetElementHandler(parser, PlistParser::start, PlistParser::end);
        XML_SetCharacterDataHandler(parser, PlistParser::chardata);
        XML_SetUserData(parser, this);
        XML_Parse(parser, buf, s, 1);
        XML_ParserFree(parser);
        return parent;
    }
        
protected:
    PlistNode parent;
    PlistNode *current;

    static void start(void *data, const char *el, const char **attr) {
        PlistParser *pr=(PlistParser*)data;
        PlistNode n(string(el), pr->current);
        pr->current = pr->current->addChild(n);
        for (int i = 0; attr[i]; i += 2) 
            pr->current->addAttr(attr[i], attr[i+1]);
    }
    
    
    static void end(void *data, const char *el) {
        PlistParser *pr=(PlistParser*)data;
        PlistNode *p = pr->current->parent();
        if (!p) pr->current = &(pr->parent); else pr->current=p;
    }
    
    
    static void chardata(void *data, const XML_Char *s, int len) {
        PlistParser *pr=(PlistParser*)data;
        for (int i=0; i<len; i++) pr->current->addText(s[i]);
    }
};

#endif