#define OVDEBUG

#include <OpenVanilla/OpenVanilla.h>
#include<OpenVanilla/OVLibrary.h>
#include<OpenVanilla/OVUtility.h>

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        printf ("usage: AddDict plistfile configname keyname type value");
        return 0;
    }

    OVAutoDeletePool pool;

    VXConfig conf(argv[1]);
    conf.read();
    
    VXDictionary *dict=conf.getDictionaryRoot();
    
    if (!dict->keyExist(argv[2])) dict->newDictionary(argv[2]);

    OVDictionary *sub=(OVDictionary*)pool.add(dict->getDictionary(argv[2]));
    
    if (!strcasecmp(argv[4], "integer"))
        sub->setInt(argv[3], atoi(argv[5]));        
    else
        sub->setString(argv[3], argv[5]);

    conf.write();
    
    return 0;
}
