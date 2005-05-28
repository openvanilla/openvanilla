// OVCandidateList.cpp

#include "OpenVanilla.h"
#include "OVUtility.h"
#include "OVCandidateList.h"
#include "OVCIN.h"
#include <iostream>

using namespace std;

void OVCandidateList::prepare(vector<string>* l, char* skey, OVCandidate *textbar)
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

void OVCandidateList::update(OVCandidate *textbar)
{
    char buf[256];
    int bound=pos+perpage;
    if (bound > count) bound=count;
    
    textbar->clear();
    
    for (int i=pos, j=0; i<bound; i++, j++)
    {
        sprintf (buf, "%c.", selkey[j]);
        textbar->append(buf)->
			append(const_cast<char*>(list->at(i).c_str()))->
            append(" ");
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

OVCandidateList* OVCandidateList::pageUp()
{
    pos-=perpage;
    if (pos < 0) pos=0;
    return this;
}

OVCandidateList* OVCandidateList::pageDown()
{
    pos+=perpage;  
    if (pos >= count) pos=0;
    return this;
}

bool OVCandidateList::select(char inKey, string& outStringRef)
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
