#ifndef __CONDITION_H
#define __CONDITION_H

#include "../CLConfig.h"

/*
To enable condition debugging uncomment _CND_DEBUG in CLConfig.h
*/


/* This macro is always defined */
#define CND_EXIT(usermessage)    CND__EXIT(__FILE__,__LINE__,CND__STR_EXIT,usermessage)

#ifndef _CND_DEBUG	/* Don't include the debug code */
    #define CND_PRECONDITION(cond, usermessage)
    #define CND_CONDITION(cond, usermessage)
    #define CND_WARNING(cond,usermessage)
    #define CND_MESSAGE(cond,usermessage)
    #define CND_DEBUGMESSAGE(usermessage)
#else 

    #ifndef CND_STR_DEFINES
        #define CND_STR_DEFINES
        #define CND_STR_PRECONDITION								1
        #define CND_STR_CONDITION									2
        #define CND_STR_WARNING										3
        #define CND_STR_MESSAGE										4
        #define CND_STR_DEBUGMESSAGE								5
        #define CND_STR_EXIT										6
    #endif

    /* _CND_DEBUG defined, include debug code */

	#ifdef _CND_NODEBUGTEXT
		#define CND_PRECONDITION(cond,usermessage)				CND__EXITCONDITION(cond,__FILE__,__LINE__,CND_STR_PRECONDITION,NULL)
		#define CND_CONDITION(cond,usermessage)					CND__EXITCONDITION(cond,__FILE__,__LINE__,CND_STR_CONDITION,NULL)
		#define CND_WARNING(cond,usermessage)					CND__CONDITION(cond,__FILE__,__LINE__,CND_STR_WARNING,NULL)
		#define CND_MESSAGE(cond,usermessage)					CND__CONDITION(cond,__FILE__,__LINE__,CND_STR_MESSAGE,NULL)
		#define CND_DEBUGMESSAGE(usermessage)					CND__MESSAGE(__FILE__,__LINE__,CND_STR_DEBUGMESSAGE,NULL)
	#else
		#define CND_PRECONDITION(cond,usermessage)				CND__EXITCONDITION(cond,__FILE__,__LINE__,CND_STR_PRECONDITION,usermessage)
		#define CND_CONDITION(cond,usermessage)					CND__EXITCONDITION(cond,__FILE__,__LINE__,CND_STR_CONDITION,usermessage)
		#define CND_WARNING(cond,usermessage)					CND__CONDITION(cond,__FILE__,__LINE__,CND_STR_WARNING,usermessage)
		#define CND_MESSAGE(cond,usermessage)					CND__CONDITION(cond,__FILE__,__LINE__,CND_STR_MESSAGE,usermessage)
		#define CND_DEBUGMESSAGE(usermessage)					CND__MESSAGE(__FILE__,__LINE__,CND_STR_DEBUGMESSAGE,usermessage)
	#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
	#include <stdio.h>
	#include <stdlib.h>
	void OutUnix( const char* File, int Line, int Title, const char *Mes2 );
	#define CND__EXIT(file,line,title,mes2)						{OutUnix(file,line,title,mes2);exit(0);}
	#define CND__EXITCONDITION(cond,file,line,title,mes2)		{if(!(cond)){OutUnix(file,line,title,mes2);exit(0);}}
	#define CND__CONDITION(cond,file,line,title,mes2)			{if(!(cond)){OutUnix(file,line,title,mes2);}}
	#define CND__MESSAGE(file,line,title,mes2)					{OutUnix(file,line,title,mes2);}
#else
    #ifdef _WIN32
	  #ifndef __WINDOWS_H
	  #include <windows.h>
	  #endif
		/* Windows platform */
		void OutDebugWindows( const char* File, int Line, int Title, const char *Mes2, int StopIcon );
		#define CND__EXIT(file,line,title,mes2)						{OutDebugWindows(file,line,title,mes2,1);FatalExit(1);}
		#define CND__EXITCONDITION(cond,file,line,title,mes2)		{if(!(cond)){OutDebugWindows(file,line,title,mes2,1);FatalExit(1);}}
		#define CND__CONDITION(cond,file,line,title,mes2)			{if(!(cond)){OutDebugWindows(file,line,title,mes2,0);}}
		#define CND__MESSAGE(file,line,title,mes2)					{OutDebugWindows(file,line,title,mes2,0);}
    #endif
#endif

#ifdef __cplusplus
}
#endif


#endif
