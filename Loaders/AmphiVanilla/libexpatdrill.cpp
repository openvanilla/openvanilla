#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <expat.h>

using namespace std;

class Node
{
public:
    Node(const char *n="", Node *p=NULL) { name=n; parent = p;}
    ~Node() {}
    
    Node(const Node& n) { name=n.name; text=n.text; children=n.children; parent=n.parent; }
    
    const Node& operator=(const Node& n) { name=n.name; text=n.text; children=n.children; parent=n.parent; } 
  
    Node* addChild(const Node& n) { 
        children.push_back(n);
        return &(children[children.size()-1]);
    }
    
    void dump() {
        int i;
        
        // printf ("Node %s=%s\n", name.c_str(), text.c_str());
        
        if (children.size()) {
            if (!parent) {
                printf ("%s", xmlhead.c_str());
            }
            else {
                spacer();
                startTag();
            }
            
            Node::dump_depth++;
            for (i=0; i<children.size();i++) children[i].dump();
            Node::dump_depth--;
            
            if (parent) {
                spacer();
                printf ("</%s>\n", name.c_str());
            }
        }
        else {
            if (!parent) {
                printf ("%s", xmlhead.c_str());
            }
            else {
                spacer();
                if (text.length()) {
                    printf ("<%s>%s</%s>\n", name.c_str(), text.c_str(), name.c_str());
                }
                else {
                    startTag();
                }
            }
        }
    }
    
    Node *getParent() { return parent; }
    
    void addText(char c) {
        text += string(1, c);
    }
    
    void addAttr(const char *k, const char *v) {
        pair<string, string> p;
        p.first = string(k);
        p.second = string(v);
        attributes.push_back(p);
    }
    
    
protected:
    void startTag() {
        printf ("<%s", name.c_str());    
        
        if (attributes.size()) {
            for (int i=0; i<attributes.size(); i++) {
                printf (" %s=\"%s\"", attributes[i].first.c_str(), 
                    attributes[i].second.c_str());
            }
        }

        if (!text.length() && !children.size()) printf ("/");
        printf (">\n");
    }
    void spacer() {
        for (int i=0;i<Node::dump_depth;i++) printf("\t");    
    }

    string name;
    string text;
    vector<Node> children;
    vector<pair<string, string> > attributes;
    Node *parent;
    
    static int dump_depth;
    static string xmlhead;
};

int Node::dump_depth = -1;

string Node::xmlhead = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";

Node parent;
Node* current = &parent;

void start(void *data, const char *el, const char **attr) {
    Node n(el, current);
    
    current = current->addChild(n);
    
    for (int i = 0; attr[i]; i += 2) {
        current->addAttr(attr[i], attr[i+1]);
    }
}


void end(void *data, const char *el) {
    Node *p = current->getParent();
    if (!p) current = &parent; else current=p;
}


void charac(void *data, const XML_Char *s, int len) {
    for (int i=0; i<len; i++) current->addText(s[i]);
}

int main() {
    FILE *f = fopen("./org.openvanilla.072.plist", "r");
    
    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    
    char *buf = (char*)calloc(1, size);
    fread(buf, size, 1, f);
    fclose(f);
    
    // printf("%s", buf);
    
    XML_Parser parser = XML_ParserCreate("UTF-8");
    XML_SetElementHandler(parser, start, end);
    XML_SetCharacterDataHandler(parser, charac);
    XML_Parse(parser, buf, size	, 1);
    XML_ParserFree(parser);
    
    free(buf);
    
    parent.dump();    
}

