class PVPhonetic : public PlainVanillaIM
{
public:
protected:
    int phonelayout;
    int candiatemode;
    OVCandidateList *candidatelist;
    VPSequence *phonesequence;
}

int PVPhonetic::keyevent(OVKeyCode *key, OVBufferService *buf, OVTextBar *textbar, OVSystemService *sys)
{
    if (candidatemode)
    {
        int refreshcandidate=0;
        int refreshnewcode=0;

        // (ESC, BACKSPACE) cancels the candidate window
        if (key->iscode(ovkEscape, ovkBackspace))
        {
            candiatemode=0;
            textbar->hide();
            buf->clear();
            buf->updatedisplay();
            return 1;       // processed
        }
        
        // (SPACE, LEFT, DOWN, '>') turns the page down
        if (key->iscode(ovkSpace, ovkLeft, ovkDown, '>'))
        {
            candidatelist->pagedown();
            refreshcandidate=1;
        }
        
        // (RIGHT, UP, '<') turns the page up
        if (key->iscode(ovkRight, ovkUp, '<'))
        {
            candidatelist->pageup();
            refreshcandiate=1;
        }

        if (refreshcandidate)
        {
            // TODO: UPDATE CANDIATE CONTENT AND REDRAW TEXTBAR            
            return 1;   // key processed
        }        
                
        if (TODO_ISNOTCANDIDATEKEY && TODO_MEANINGFULKEY)
        {
            // TODO: USE THE FIRST CANDIDATE
            refreshnewcode=1;    // uses the meaningful key as the next new character in phonesequence
        }

        if (key->iscode(ovkReturn, ovkLinefeed))
        {
            // TODO: USE THE FIRST CANDIDATE
        }        
        
        if (TODO_ISCANDIDATEKEY)
        {
            // TODO: SELECT THE CANDIDATE
            // TODO: COPY THE CANDIDATE INTO WINDOW
            candiatemode=0;
            textbar->hide();
            buf->clear();
            buf->addstring(TODO_COPY-THE_CANDIDATE);
            buf->send();
            
            if (refreshnewcode)
            {
                // TODO: GET THE CORRESPONDING SYMBOL INTO PHONESEQUENCE
                buf->clear();
                buf->addstring(TODO_COPY-THE_SYMBOL);
                buf->updatedisplay();
            }
        }
        
        return 1;       // in any case, the key is processed
    }

    // if there is nothing in phonesequence, process the follwing keys
    if (TODO_IF_NOTHING_IN_PHONESEQUENCE)
    {
        // "throw back" all command/ctrl/opt key combinations
        if (key->iscommand() || key->isctrl() || key->isopt())
        {
            return 0;   // not processed, tell the app to process it
        }		

        if (key->iscapslock())
        {
            if (key->isshift()) // TODO: TOUPPER KEYCODE
            else // TODO: TOLOWER KEYCODE
         
            // TODO: COPY THE 
            buf->clear();
            buf->copystr(TODO_COPY-THE-KEYCODE);
            buf->send();
            return 1;   
		}
		
		// send back RETURN/ENTER (linefeed)/SPACE/BACKSPACE/DELETE/ESCAPE
		if (key->iscode(ovReturn, ovLinefeed, ovSpace, ovBackspace, 
            ovDelete, ovEscape) || !key->isprintable())
        {
            return 0;  
        }
		 
        // process punctuation marks
        if (TODO_IF_PHONE_PUNCTUATION_MARK)
        {
            // TODO_IF_ONLY_ONE_CANDIDATE
            if (TODO_IF_ONLY_ONE_CANDIDATE)
            {
                buf->clear();
                buf->addstring(TODO_SYMBOL);
                buf->send();
                return 1;
            }
            else
            {
                // TODO: LOAD CANDIDATE LIST
                // TODO: SHOW TEXTBAR
                candidatemode=1;
                
                buf->clear();
                buf->copystr(TODO_COPY_FIRST_CANDIDATE);
                buf->updatedisplay();
                return 1;   // key processed
            }            
        }   // of if ()
    }   //of if()
    
    if (TODO_SOMETHING_IN_PHONESEQUENCE)
    {
        int hittone=0;
        
        if (TODO_HIT_TONEMARK)
        {
            hittone=1;
            // TODO: DRAW TONE MARK
            buf->clear();
            buf->addstring(TODO_PHONESYMBOL);
            buf->updatedisplay();
        }

        if (hittone || key->iscode(ovkEnter, ovkLinefeed, ovkSpace))
        {
            // TODO: SEARCH CANDIDATE!
            // TODO: IF NO CANDIDATE, WARNING SOUND
            
            if (TODO_NOCANDIDATE)
            {
                sys->warningbeep();
                return 1;
            }
        
            // TODO: WASH AWAY CURRENT PHONE SEQUENCE

            if (TODO_ONLYONECANDIDATE)
            {
                buf->clear();
                buf->addstring(TODO_COPYCANDIDATE);
                buf->send();
            }            
            else
            {
                buf->clear();
                buf->addstring(TODO_FIRSTCANDIDATE);
                buf->updatedisplay();
                
                // TODO: BRING UP CANDIDATE WINDOW AND DRAW
                candidatemode=1;                  
            }
            
            return 1;       // processed
        }                    
    }   // of if()    
    

    if (key->iscode(ovkEscape))
    {
        // no worry, this ESCAPE occurs when something is in the phonesequence
        buf->clear();
        buf->updatedisplay();
        return 1;       
    }    
    else if (key->iscode(ovkBackspace))
    {
        // TODO: REMOVE THE LAST SYMBOL OF THE PHONESEQUENCE
        if (TODO_PHONESEQUENCEEMPTY)
        {
            buf->clear();
            buf->updatedisplay();
        }
    }
    else
    {   
        // TODO: ADD KEY TO PHONESEQUENCE
    }   // of if ()... else if ()... else
    
    // TODO: REFRESH PHONESEQUENCE DISPLAY
    if (TODO_SEQUENCE)
    {
        buf->clear();
        buf->addstring(TODO_SEQUENCE);
        buf->updatedisplay();
    }
    else
    {
        // if there is still nothing in the phonesequence, just send the key
        buf->clear();
        buf->addstring(TODO_KEY);
        buf->send();        
    }
    
    return 1;
}
