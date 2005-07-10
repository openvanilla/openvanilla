#ifndef LuceneApple_H
#define LuceneApple_H
#ifdef _APPLE_

///////////////////////////////////////////////////////////////////
//
// now we know we are Apple system.
//

#include "config.h"

#undef _T //apple has something else defined here which stuffs up the compile
#define _UNICODE //default to unix
#include "CLucene/LuceneUnix.h" //mostly unix compatible

#endif
