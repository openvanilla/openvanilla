
#include "OpenVanilla.h"
#include "OVUtility.h"
#include "VXTextBar.h"

#include <string.h>
#include <unistd.h>

int main(void) {
    VXTextBar bar;
    char line[1024];
    while(fgets(line,1023,stdin)) {
	murmur("%s",line);
	bar.hide();
        bar.clear();
	bar.append((char*)line,ovEncodingUTF8);
	bar.update();
	bar.show();
	murmur("----------------");
	sleep(1);
    }
}

