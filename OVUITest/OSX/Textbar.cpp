
#include "OpenVanilla.h"
#include "OVUtility.h"
#include "VXTextBar.h"

#include <string.h>
#include <unistd.h>

char *str[] = 
{
    "Quick",
    "Quick Fox",
    "Quick Fox Jump",
    "Quick Fox Jump Over",
    "Quick Fox Jump Over Lazy",
    "Quick Fox Jump Over Lazy Sleeping",
    "Quick Fox Jump Over Lazy Sleeping Rabbit",

    "一",
    "一二",
    "一二三",
    "一二三四",
    "一二三四五",

    "a. 一",
    "a. 一 b. 二",
    "a. 一 b. 二 c. 三",
    "a. 一 b. 二 c. 三 d. 四",
    "a. 一 b. 二 c. 三 d. 四 e. 五",

    "\0",
};

int main(void) {
    VXTextBar bar;
    for(int t = 0; str[t][0] != 0 ; t++) {
	murmur("%s",str[t]);
	bar.hide();
        bar.clear();
	bar.append((char*)str[t],ovEncodingUTF8);
	bar.update();
	bar.show();
	murmur("----------------");
	sleep(1);
    }
}

