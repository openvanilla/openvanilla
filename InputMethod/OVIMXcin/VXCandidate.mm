// VXCandidate.cpp

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include "VXCandidate.h"

void VXCandidate::prepare(NSArray *l, char *skey, OVTextBar *textbar)
{
    onduty=1;
    list=l;
    strcpy(selkey, skey);
    perpage=strlen(selkey);
    pos=0;
    count=[list count];
    
    murmur ("prepare, selkey=%s, perpage=%d, pos=%d, count=%d",
        selkey, perpage, pos, count);
    
    update(textbar);
    textbar->show();
}

void VXCandidate::update(OVTextBar *textbar)
{
    char buf[256];
    int bound=pos+perpage;
    if (bound > count) bound=count;
    
    textbar->clear();
    
    for (int i=pos, j=0; i<bound; i++, j++)
    {
        sprintf (buf, "%c.", selkey[j]);
        textbar->append(buf)->append((void*)[[list objectAtIndex: i] UTF8String])->
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

VXCandidate* VXCandidate::pageUp()
{
    pos-=perpage;
    if (pos < 0) pos=0;
    return this;
}

VXCandidate* VXCandidate::pageDown()
{
    pos+=perpage;  
    if (pos >= count) pos=0;
    return this;
}

NSString* VXCandidate::select(char c)
{
    int i;
    for (i=0; i<perpage; i++)
        if (selkey[i]==c && (pos+i < count))
        {
            onduty=0;
            return [list objectAtIndex: pos+i];
        }
        
    return NULL;        
}
