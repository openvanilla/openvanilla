// OVCandidate.cpp

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include "OVCandidate.h"

void OVCandidate::prepare(vector<string> *l, char *skey, OVTextBar *textbar)
{
    onduty=1;
    list=l;
    strcpy(selkey, skey);
    perpage=strlen(selkey);
    pos=0;
    count=list->size();
    
    murmur ("prepare, selkey=%s, perpage=%d, pos=%d, count=%d",
        selkey, perpage, pos, count);
    
    update(textbar);
    textbar->show();
}

void OVCandidate::update(OVTextBar *textbar)
{
    char buf[256];
    int bound=pos+perpage;
    if (bound > count) bound=count;
    
    textbar->clear();
    
    for (int i=pos, j=0; i<bound; i++, j++)
    {
        sprintf (buf, "%c.", selkey[j]);
        textbar->append(buf)->append((void*)list->at(i))->
            append((void*)" ");
    }
    
    int totalpage=(count % perpage) ? (count/perpage)+1 : (count/perpage);
    int currentpage=(pos/perpage)+1;
//  if (totalpage >1)
//  {
        sprintf (buf, "(%d/%d)", currentpage, totalpage);
        textbar->append(buf);
//  }
    textbar->update();   
}

OVCandidate* OVCandidate::pageUp()
{
    pos-=perpage;
    if (pos < 0) pos=0;
    return this;
}

OVCandidate* OVCandidate::pageDown()
{
    pos+=perpage;  
    if (pos >= count) pos=0;
    return this;
}

string* OVCandidate::select(char c)
{
    int i;
    for (i=0; i<perpage; i++)
        if (selkey[i]==c && (pos+i < count))
        {
            onduty=0;
            return &(list->at(pos+i));
        }
        
    return NULL;        
}
