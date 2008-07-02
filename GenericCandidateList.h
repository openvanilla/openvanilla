#ifndef __GenericCandidateList_h
#define __GenericCandidateList_h

#include "OVCandidateList.h"



class GenericCandidateList : public OVCandidateList {
public:
#if defined(WIN32)&&!defined(WINCE)
	virtual void prepare(vector<string>* l, char* skey, OVCandidate *textbar, char* dkey=NULL, string c=string("") )
{
    onduty=true;
    
	strcpy(selkey, skey);

	if(dkey)
		strcpy(displaykey, dkey);
	else
		strcpy(displaykey, "");
	
	
	caption = c;

    perpage=static_cast<int>(strlen(selkey));
    pos=0;
	list = l;	
    count=static_cast<int>(list->size());
        
    update(textbar);
    textbar->show();
}

    virtual void update(OVCandidate *textbar) {

    char buf[256];
    int bound=pos+perpage;
    if (bound > count) bound=count;
    
    textbar->clear();
    
    for (int i=pos, j=0; i<bound; i++, j++)
    {
		if(strcmp(displaykey,""))
			sprintf (buf, "%c%c", displaykey[j], selkey[j]);
		else
			sprintf (buf, "%c.", selkey[j]);
			

        textbar->append(buf)->
			append(const_cast<char*>(list->at(i).c_str()))->
            append(" ");
    }
    
    int totalpage=(count % perpage) ? (count/perpage)+1 : (count/perpage);
    int currentpage=(pos/perpage)+1;

	sprintf (buf, "%s(%d/%d)", caption.c_str(), currentpage, totalpage);
        textbar->append(buf);

    textbar->update();   
    }

	virtual bool select(char inKey, string& outStringRef)
{
    int i;
	char c;
    for (i=0; i<perpage; i++)
	{

		if(selkey[i]=='.') c= displaykey[i];
		else c= selkey[i];

		if ( c ==inKey && (pos+i < count)) {
            onduty=false;
            outStringRef = list->at(pos+i);
			return true;
        }
	}
	
    return false;
}


#elif defined(WINCE)


bool uiselect(OVKeyCode *inKey, string& outStringRef)
{
	if( inKey->isAlt() && inKey->isShift() && inKey->isCtrl() ) 
	{
		onduty=false;
		outStringRef = list->at(inKey->isNum());
		return true;
	}
	
    
    return false;
}

virtual void update(OVCandidate *textbar) {

    char buf[256];
	int bound= list->size();   //pos+perpage;
    if (bound > 200) bound=200;
	count=bound;
    
    textbar->clear();
     
    for (int i=0; i<bound; i++)
    {
		//sprintf (buf, "%c.", selkey[i% strlen(selkey)]);
        //textbar->append(buf)->
		textbar->append(const_cast<char*>(list->at(i).c_str()))->
            append(" ");

    }
	sprintf (buf, "..(1/%d)", bound);
    textbar->append(buf);

	textbar->update();   

}

#endif 

protected:
    char displaykey[32];
	std::string caption;


};

#endif
