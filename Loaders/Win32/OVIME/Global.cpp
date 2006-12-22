#include "OVIME.h"
#include <stdio.h>

HINSTANCE hInst;
int CompX;
int CompY;
int CandX;
int CandY;
bool isChinese = true;
bool isFullShape = false;
//bool isActive=true;
DisplayServer *dsvr = new DisplayServer();
AVLoader* loader;
