
#define OVDEBUG 1

#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVUtility.h>
#include "../../Loaders/OSX/VXTextBar.h"

#include <string.h>
#include <unistd.h>

int main(void) {
    VXTextBar bar;
    char line[1024];
    while(fgets(line,1023,stdin)) {
        if (line[strlen(line)-1]==0xa) line[strlen(line)-1]=0;
        murmur("len=%d, %s", strlen(line), line);
        bar.hide();
        bar.clear();
        bar.append((char*)line,ovEncodingUTF8);
        bar.update();
        bar.show();
        bar.update();
        murmur("----------------");
        sleep(1);
    }
}
