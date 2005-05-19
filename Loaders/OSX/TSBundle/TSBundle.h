// TSB.H: Header file for dynamically-loadable text service bundle

#ifndef __TSBundle_H
#define __TSBundle_H

#define TSBUNDLEFILENAME    "/Library/OpenVanilla/0.7.0/OVLoader/OVLoader.bundle"
#define TSBUNDLEID          "org.openvanilla.ovloader.070rc4"

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
