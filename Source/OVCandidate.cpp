// OVCandidate.cpp

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include "OVCandidate.h"
#include "OVCIN.h"
#include <iostream>

using namespace std;

void OVCandidate::prepare(vector<string>* l, char* skey, OVTextBar *textbar)
{
    onduty=1;
    strcpy(selkey, skey);
    perpage=strlen(selkey);
    pos=0;
	list = l;	
    count=list->size();
        
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
        textbar->append(buf)->
			append((void*)const_cast<char*>(list->at(i).c_str()))->
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

bool OVCandidate::select(char inKey, string& outStringRef)
{
    int i;
    for (i=0; i<perpage; i++)
	{
        if (selkey[i]==inKey && (pos+i < count)) {
            onduty=0;
            outStringRef = list->at(pos+i);
			return true;
        }
	}
	
    return false;
}
