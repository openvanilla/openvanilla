// TSBundle.h: Header file for dynamically-loadable text service bundle
//
// This implementation loads CocoaVanilla (OVLoader.bundle)
//
// Copyright (c) 2004-2006 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __TSBundle_H
#define __TSBundle_H

#define TSBUNDLEFILENAME    "/Library/OpenVanilla/0.8.0/Loader/OVLoader.bundle"
#define TSBUNDLEID          "org.openvanilla.ovloader.080"

#ifdef __cplusplus
extern "C" {
#endif
    typedef void *TSBDataPtr;

    ComponentResult TSBInitialize(MenuRef mnu);
    ComponentResult TSBContextOpen(ComponentInstance inst, TSBDataPtr *savePtr);
    ComponentResult TSBContextClose(TSBDataPtr ptr);
    ComponentResult TSBContextActivate(TSBDataPtr ptr);
    ComponentResult TSBContextDeactivate(TSBDataPtr ptr);
    ComponentResult TSBContextFix(TSBDataPtr ptr);
    ComponentResult TSBContextEvent(TSBDataPtr ptr, EventRef evnt);
    ComponentResult TSBMenuHandler(UInt32 cmd);

    typedef ComponentResult _tsbinit_t(MenuRef m);
    typedef ComponentResult _tsbopen_t(ComponentInstance i, TSBDataPtr *savePtr);
    typedef ComponentResult _tsbfunc_t(TSBDataPtr ptr);
    typedef ComponentResult _tsbevent_t(TSBDataPtr ptr, EventRef e);
    typedef ComponentResult _tsbmenu_t(UInt32 cmd);
#ifdef __cplusplus
};
#endif

#endif
