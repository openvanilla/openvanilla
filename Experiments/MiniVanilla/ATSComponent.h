// ATSComponent.h: text service component constants

// The constants defined here are only meaningful for ATSComponent.r

#ifndef __ATSComponent_h
#define __ATSComponent_h

#define ATSCVERSION             0x00010000
#define ATSCSCRIPT              0x7e          // 2 = smTradCinese
#define ATSCLANGUAGE            0          // 19 = langtradChinese, 0x4a=tamil


// #define ATSCBASERESOURCEID      16896       // (15872+script_id*512) here script_id=smTradChinese
#define ATSCBASERESOURCEID		0xa600		// trying

#define ATSCPENCILMENUID        ATSCBASERESOURCEID+1
#define ATSCBUNDLENAME          "org.openvanilla.mvinit"
#define ATSCBNLENGTH            $"16"       // in hexadecimal
#define ATSCIMNAME              "MiniVanilla Loader 0.1.0"
#define ATSCIMNLENGTH           $"18"       // in hexadecimal
#define ATSCVENDORCODE          'opvn'

#endif
