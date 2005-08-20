#include "OVDisplayServer.h"
#include "OVDisplayComponent.h"

void OVDisplayComponent::regDisplayServer(OVDisplayServer *ds) { svr = ds; svr->regDisplayComponent(this); }