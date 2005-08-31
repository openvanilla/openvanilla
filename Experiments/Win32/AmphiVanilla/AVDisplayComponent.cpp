#include "AVDisplayServer.h"
#include "AVDisplayComponent.h"

void OVDisplayComponent::regDisplayServer(OVDisplayServer *ds) { svr = ds; svr->regDisplayComponent(this); }
