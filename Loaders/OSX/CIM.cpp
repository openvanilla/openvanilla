// CIM.c

#include <Carbon/Carbon.h>
#include <stdarg.h>

#include "CIM.h"
#include "CIMconst.h"

const short cimMenu_Pencil=cimBaseResourceID+1;

CIMSessionHandle cimActiveSession;
MenuRef cimPencilMenu;

pascal OSStatus CIMPencilMenuHandler(EventHandlerCallRef callref,
    EventRef evnt, void *userdata);

// initialize global state
ComponentResult CIMInitialize(ComponentInstance inst, MenuRef *mnu)
{
    ComponentResult result=noErr;
    short ref=-1;
    EventTypeSpec eventspec;

    cimActiveSession=NULL;
    cimPencilMenu=NULL;

    // open resource fork
    if (result==noErr)
    {
        ref=OpenComponentResFile((Component)inst);
        result=ResError();
        if ((result==noErr) && (ref==-1)) result=resFNotFound;
    }
	
    // load the pencil menu
    if (result==noErr)
    {
        cimPencilMenu=GetMenu(cimMenu_Pencil);
        if (cimPencilMenu) *mnu=cimPencilMenu;
        else result=resNotFound;
    }    

    // according to Apple's BIM, this indicates that "a pencil glyph
    // should be substitued for the standard control key glyph (for JIS keyboard)
    if (result==noErr)
        ChangeMenuAttributes(cimPencilMenu, kMenuAttrUsePencilGlyph, 0);    

    // install menu event handler
    if (result==noErr)
    {
        eventspec.eventClass=kEventClassCommand;
        eventspec.eventKind=kEventProcessCommand;
        result=InstallMenuEventHandler(cimPencilMenu,
            NewEventHandlerUPP(CIMPencilMenuHandler), 1, &eventspec, nil, nil);
    }
    	
	// set menu item strings
	if (result==noErr)
	{
		// load menu item here

        // if you want to load a localized string, insert the following code
        // and changed the CAPITALIZED part:
		//     CFBundleRef bundle=CFBundleGetBundleWithIdentifier(BUNDLENAME);
        //     CFStringRef ref;
        //     ref=CFBundleCopyLocalizedString(bundle, "NAME", NULL NULL);

        // InsertMenuItemTextWithCFString(MENU, STRREF, AFTERPOS, ATTR, CMDID);
		#ifndef CIMCUSTOM
			InsertMenuItemTextWithCFString(cimPencilMenu, 
				CFSTR("About CIM..."), 0, 0, 1); 
		#endif
    }

    // close resource fork
    if (ref!=-1) CloseComponentResFile(ref);
	
    // launch IM server (not implemented here)
    // result=CIMLaunchServer();
    // if (result==noErr) result=CIMInitializeMessageReceiving();    
    
	#ifdef CIMCUSTOM
		if (CIMCustomInitialize(cimPencilMenu)) result=noErr; 
		else result=invalidHandler;
	#endif
	
    return result;
}


void CIMTerminate(ComponentInstance inst)
{
    cimActiveSession=NULL;
    cimPencilMenu=NULL;
	
	#ifdef CIMCUSTOM
		CIMCustomTerminate();
	#endif
}


ComponentResult CIMSessionOpen(ComponentInstance inst, 
    CIMSessionHandle *hndl)
{
    ComponentResult result=noErr;

    if (*hndl==nil)
    {
        *hndl=(CIMSessionHandle)NewHandle(sizeof(CIMSessionData));

        if (*hndl)
		{
			(**hndl)->init(inst);
			#ifdef CIMCUSTOM
				(**hndl)->data=CIMCustomOpen();
			#endif
		}
		else result=memFullErr;
    }

    return result;
}

void CIMSessionClose(CIMSessionHandle hndl)
{
    if (hndl)
    {
        (*hndl)->kill();
		#ifdef CIMCUSTOM
			if ((*hndl)->data) CIMCustomClose((*hndl)->data);
		#endif
        DisposeHandle((Handle)hndl);
    }
}

ComponentResult CIMSessionActivate(CIMSessionHandle hndl)
{
    OSStatus result=noErr;
   
    cimActiveSession=hndl;
    
    // tell IM server (not implemented here) that we are activated
    // result=CIMSendActivatedMessage();
    
	#ifdef CIMCUSTOM
		if (CIMCustomActivate((*hndl)->data,
			(*hndl)->buffer->bind((*hndl)->instance)))
				result=noErr;
		else result=invalidHandler;
	#endif
	
    return result;
}

ComponentResult CIMSessionDeactivate(CIMSessionHandle hndl)
{
    cimActiveSession=nil;
	    
    // tell IM server (not implemented here) that we are deactivated
    // return CIMSendDeactivatedMessage();
    
	#ifdef CIMCUSTOM
		if (!CIMCustomDeactivate((*hndl)->data,
			(*hndl)->buffer->bind((*hndl)->instance)))
				return invalidHandler;
	#endif

    return noErr;	
}

ComponentResult CIMSessionFix(CIMSessionHandle hndl)
{
    if ((*hndl)->sessionFixLock) return noErr;
    
	#ifdef CIMCUSTOM
		if (!CIMCustomSessionFix((*hndl)->data,
			(*hndl)->buffer->bind((*hndl)->instance)))
				return invalidHandler;
	#else
        (*hndl)->buffer->bind((*hndl)->instance)->send();
    #endif
    
    return noErr;
}

ComponentResult CIMSessionHidePalettes(CIMSessionHandle hndl)
{
	return noErr;
}

ComponentResult CIMSessionEvent(CIMSessionHandle hndl, EventRef evnt)
{
    UInt32 eventkind=GetEventKind(evnt);

    // we only take kEventRawKeyDown and kEventRawKeyRepat
    if (GetEventClass(evnt)!=kEventClassKeyboard ||
        !(eventkind==kEventRawKeyDown || eventkind==kEventRawKeyRepeat))
        return FALSE;
        
    UInt32 keycode, modifiers;
    unsigned char charcode;

    // extract the key code, charcode, and modifiers 
    GetEventParameter(evnt, kEventParamKeyCode, typeUInt32, nil,
        sizeof(keycode), nil, &keycode);
    GetEventParameter(evnt, kEventParamKeyMacCharCodes, typeChar, nil,
        sizeof(charcode), nil, &charcode);
    GetEventParameter(evnt, kEventParamKeyModifiers, typeUInt32, nil,
        sizeof(modifiers), nil, &modifiers);

    (*hndl)->sessionFixLock=1;
    
	#ifdef CIMCUSTOM
		Point p;
		CIMGetInputPosition(hndl, &p);
	
		int x=CIMCustomHandleInput((*hndl)->data, 
			(*hndl)->buffer->bind((*hndl)->instance),
			charcode, keycode, modifiers, &p);
	#else
		int x=CIMHandleInput((*hndl)->buffer->bind((*hndl)->instance),
			charcode, keycode, modifiers);
	#endif
	
	(*hndl)->sessionFixLock=0;
	return x;
}


// this demonstrates a basic input method
int CIMHandleInput(CIMInputBuffer *buf, unsigned char charcode,
    UInt32 keycode, UInt32 modifiers)
{
	// put printable character into the input buffer, update the display,
	// and returns 1 -- telling app that we have "eaten" that character
	if (isprint(charcode))
	{
		buf->put(charcode);
		buf->update();
		return 1;
	}
	
	// this section handles function keys
    switch (charcode)
    {
        case 0x08:  // BACKSPACE
		case 0x7f:	// DELETE
			if (!buf->length()) return 0;
            buf->deletechar();
            buf->update();
            return 1;

        case 0x03:  // Mac's ENTER (but not LF)
        case 0x0D:  // Mac's RETURN (carrige return?)
            if (buf->length())	// if buffer not empty, "send" all chars
			{
				buf->send();
				return 1;
			}
			return 0;
            
		case 0x1b:	// ESC
			if (buf->length())
			{
				buf->clear();
				buf->update();
				return 1;
			}
			return 0;
    }
    
    return 0;	// send back all unprocessed keys
}


pascal OSStatus CIMPencilMenuHandler(EventHandlerCallRef callref, 
    EventRef evnt, void *userdata)
{
    HICommand command;
 
	if (GetEventParameter(evnt, kEventParamDirectObject, typeHICommand, nil,
        sizeof(command), nil, &command)!=noErr) return eventNotHandledErr;
        
	#ifdef CIMCUSTOM
		if (CIMCustomMenuHandler((*cimActiveSession)->data, command.commandID, 
            cimPencilMenu, 
            (*cimActiveSession)->buffer->bind((*cimActiveSession)->instance)))
                return noErr;
	#else
		switch(command.commandID)
		{
			case 1:
				fprintf (stderr, "about\n");
				return noErr;
		}
	#endif

    return eventNotHandledErr;    
}

ComponentResult CIMGetInputPosition(CIMSessionHandle hndl, Point *pnt)
{
	
	long refcon=0, offset=0;
	Boolean edge=0;
	ComponentInstance inst=(*hndl)->instance;
	EventRef event=NULL;
	OSStatus status=CreateEvent(NULL, kEventClassTextInput, 
	   kEventTextInputOffsetToPos, GetCurrentEventTime(), 
	   kEventAttributeUserEvent, &event);

	ScriptLanguageRecord record;
	pnt->h = pnt->v = 0;
	memset(&record, sizeof(record), 0);

	#define EVENTPARAM(a, b, c, d) if (status==noErr) \
		status=SetEventParameter(event, a, b, sizeof(c), d)

	EVENTPARAM(kEventParamTextInputSendComponentInstance, 
		typeComponentInstance, ComponentInstance, &inst);
	EVENTPARAM(kEventParamTextInputSendRefCon, typeLongInteger, long, &refcon);
	EVENTPARAM(kEventParamTextInputSendTextOffset, typeLongInteger, long, &offset);
	EVENTPARAM(kEventParamTextInputSendSLRec, typeIntlWritingCode, 
		ScriptLanguageRecord, &record);
	EVENTPARAM(kEventParamTextInputSendLeadingEdge, typeBoolean, Boolean, &edge);

    if (status==noErr) status=SendTextInputEvent(event);
	
    if (status==noErr) status=GetEventParameter(event, 
        kEventParamTextInputReplyPoint, typeQDPoint, NULL, sizeof(Point), 
		NULL, pnt);    

    if (event) ReleaseEvent(event);
    
    return status;
}

int CIMInputBuffer::realpos(int p)
{
    int pp=0;
    int i;
    
    for (i=0; i<len; i++)
    {
        if (pp==p) break;

        // see if encounters a surrogate pair
        if (buffer[i] >= 0xd800 && buffer[i] <= 0xdfff)
        {
            i++;
            if (i<len && buffer[i] >= 0xd800 && buffer[i] <= 0xdfff) pp++;
        }
        else pp++;
    }
    
//	fprintf (stderr, "realpos; length=%d, request=%d, actual=%d\n", len, p, i);
	
    return i;
}

void CIMInputBuffer::deletechar()
{
    if (!len) return;
    
    len--;
    if (buffer[len] >= 0xd800 && buffer[len] <= 0xdfff) // surrogate char
    {
        if (len) len--;    
    }
}

int CIMInputBuffer::length()
{
    int l=0;
    
    for (int i=0; i<len; i++)
    {
        // see if encounters a surrogate pair
        if (buffer[i] >= 0xd800 && buffer[i] <= 0xdfff)
        {
            i++;
            if (i<len && buffer[i] >= 0xd800 && buffer[i] <= 0xdfff) l++;
        }
        else l++;
    }
    
    return l;
}

int CIMInputBuffer::put(UniChar c)
{
	if (!(len < cimIBMaxLen)) return 0;
	buffer[len++]=c;
	return 1;
}

int CIMInputBuffer::put(CFStringRef s)
{
    if (!s) return 0;
    
    int l=CFStringGetLength(s);
    int r=1;
    UniChar csbuf[cimIBMaxLen];

    CFRange rng=CFRangeMake(0, (l > cimIBMaxLen ? cimIBMaxLen : l));
    CFStringGetCharacters(s, rng, csbuf);
    for (int i=0; i<l; i++) if (!(r=put(csbuf[i]))) break;
    return r;
}

int CIMInputBuffer::put(char *s, CFStringEncoding e)
{
    CFStringRef ref=CFStringCreateWithCString(NULL, s, e);
    if (!ref) return 0;

    int r=put(ref);
    CFRelease(ref);
    return r;
}

OSErr CIMInputBuffer::send()
{
	ComponentResult r=update(TRUE);
	clear();
	return r;
}

OSErr CIMInputBuffer::update(Boolean send, int cursorpos, int hilitefrom, 
	int hiliteto, ScriptCode sc, LangCode lang)
{
    OSErr error=noErr;
    long fixlen=0;
    EventRef event;
    ScriptLanguageRecord script;
    TextRange pinrange;
    TextRangeArrayPtr hiliterangeptr=nil, updaterangeptr=nil;

    if (send) fixlen=len*sizeof(UniChar);       // why *2 ?

    // create a new Text Input event, class=kEventClassTextInput,
    // kind=kEventTextInputUpdateActiveInputArea
    error=CreateEvent(NULL, kEventClassTextInput, 
        kEventTextInputUpdateActiveInputArea,
        GetCurrentEventTime(), kEventAttributeUserEvent, &event);

    // first parameter: reference to our text service component's instance
    if (error==noErr)
    {
        error=SetEventParameter(event, kEventParamTextInputSendComponentInstance,
            typeComponentInstance, sizeof(ComponentInstance),
            &instance);
    }

    // second parameter: script information 
    if (error==noErr)
    {
        script.fScript=sc;
        script.fLanguage=lang;
        error=SetEventParameter(event, kEventParamTextInputSendSLRec,
            typeIntlWritingCode, sizeof(ScriptLanguageRecord), &script);
    }

    // 3rd parameter: what we have in the buffer
    if (error==noErr)
        error=SetEventParameter(event, kEventParamTextInputSendText,
            typeUnicodeText, len * sizeof(UniChar), buffer);
    
    
    // 4th paramter: "fix" length, >0 if we're dumping ("sending") the buffer
    if (error==noErr)
        error=SetEventParameter(event, kEventParamTextInputSendFixLen,
            typeLongInteger, sizeof(long), &fixlen);

    //  If the text hasn't been changed, then we want to pass an update handle that contains no
    //  update regions, otherwise our handle will say that everything has changed.

    // set update region (currently none)
    if(error==noErr)
    {
        updaterangeptr=(TextRangeArrayPtr)NewPtrClear(sizeof(short)+
            sizeof(TextRange)*2);
            
        if (!updaterangeptr) error=memFullErr;
        else
        {
            updaterangeptr->fNumOfRanges=2;
            updaterangeptr->fRange[0].fStart=0;
            updaterangeptr->fRange[0].fEnd=lastupdate*sizeof(UniChar);
            updaterangeptr->fRange[0].fHiliteStyle=0;
            updaterangeptr->fRange[1].fStart=0;
            updaterangeptr->fRange[1].fEnd=len*sizeof(UniChar);
            updaterangeptr->fRange[1].fHiliteStyle=0;

            lastupdate=len;
            error=SetEventParameter(event, kEventParamTextInputSendUpdateRng,
                typeTextRangeArray, sizeof(short)+sizeof(TextRange)*2,
                updaterangeptr);
        }
    }

    if(error==noErr)
    {
        hiliterangeptr=(TextRangeArrayPtr)NewPtrClear(sizeof(short)+
            sizeof(TextRange)*3);
			
		// among the four TSM update messages: k(Selected)RawText and
		// k(Selected)ConvertedText, it seems only the latter pair works,
		// i.e both kSelectedRawText and kRawText seem to be defunct
			
		#define SETRANGE(r, f, e, s) hiliterangeptr->fRange[r].fStart=f; \
			hiliterangeptr->fRange[r].fEnd=e; \
			hiliterangeptr->fRange[r].fHiliteStyle=s
			
        if(!hiliterangeptr) error=memFullErr;
        else
        {
            hiliterangeptr->fNumOfRanges=2;
			
			int realcur=len*sizeof(UniChar);
			// set cursor position
			if (cursorpos>=0 && cursorpos<=length()) 
                realcur=realpos(cursorpos)*sizeof(UniChar);

                
            SETRANGE(0, 0, len*sizeof(UniChar), kConvertedText);
			SETRANGE(1, realcur, realcur, kCaretPosition);
			
			// requests app to draw a light gray underline to our text area

			// if hitelitefrom & hiteliteto are set, draw a darker line
			if 	((hilitefrom>=0 && hilitefrom<=length()) &&
				(hiliteto>hilitefrom && hiliteto<=length()))
			{
				hiliterangeptr->fNumOfRanges++;		// send one more range block

				SETRANGE(1, realpos(hilitefrom)*sizeof(UniChar),
					realpos(hiliteto)*sizeof(UniChar), 
					kSelectedConvertedText);
			    SETRANGE(2, realcur, realcur, kCaretPosition);
			}

            
            error=SetEventParameter(event, kEventParamTextInputSendHiliteRng,
                typeTextRangeArray, 
				sizeof(short)+sizeof(TextRange)*hiliterangeptr->fNumOfRanges,
                hiliterangeptr);
        }
    }

    // we don't any "clause" information
    pinrange.fStart=0;
    pinrange.fEnd=len*sizeof(UniChar);
    if(error==noErr)
        error=SetEventParameter(event,kEventParamTextInputSendPinRng,
            typeTextRange, sizeof(TextRange), &pinrange);

    if (error==noErr) error=SendTextInputEvent(event);      // send event

    // delete the allocated range objects
    DisposePtr((Ptr)hiliterangeptr);
    DisposePtr((Ptr)updaterangeptr);
    
    return error;
}

