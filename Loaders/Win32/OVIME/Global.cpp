#include "OVIME.h"
#include <stdio.h>

HINSTANCE hInst;
int CompX;
int CompY;
int CandX;
int CandY;
bool isChinese = true;
bool isTraditional = true;
bool isFullShape = false;

DisplayServer *dsvr = new DisplayServer();
AVLoader* loader = AVLoader::getLoader();
