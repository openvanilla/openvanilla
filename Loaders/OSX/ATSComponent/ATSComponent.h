// ATSComponent.h: text service component constants

// The constants defined here are only meaningful for ATSComponent.r

#ifndef __ATSComponent_h
#define __ATSComponent_h

#define ATSCVERSION             0x00010000
#define ATSCSCRIPT              0x7e          
#define ATSCLANGUAGE            0        

#define ATSCBASERESOURCEID	0xa600		// trying

#define ATSCPENCILMENUID        ATSCBASERESOURCEID+1
#define ATSCBUNDLENAME          "org.openvanilla.ovinit"
#define ATSCBNLENGTH            $"16"       // in hexadecimal
#define ATSCIMNAME              "OpenVanilla Loader 0.7.1"
#define ATSCIMNLENGTH           $"18"       // in hexadecimal
#define ATSCVENDORCODE          'opvn'

#if defined(ppc_YES)
    // PPC architecture
    #define TARGET_REZ_MAC_PPC 1
#else
    #define TARGET_REZ_MAC_PPC 0
#endif

#if defined(i386_YES)
    // x86 architecture
    #define TARGET_REZ_MAC_X86 1
#else
    #define TARGET_REZ_MAC_X86 0
#endif

#endif
