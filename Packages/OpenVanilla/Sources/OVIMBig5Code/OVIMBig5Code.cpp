#include "OVIMBig5Code.h"
#include "OVIMBig5CodeContext.h"

using namespace OpenVanilla;

OpenVanilla::OVIMBig5Code::OVIMBig5Code() {

}

OpenVanilla::OVIMBig5Code::~OVIMBig5Code() {

}


OVEventHandlingContext *OpenVanilla::OVIMBig5Code::createContext() {
    return reinterpret_cast<OVEventHandlingContext *>(new OVIMBig5CodeContext(this));
}


const string OpenVanilla::OVIMBig5Code::identifier() const {
    return string("org.openvanilla.OVIMBig5Code");
}

const string OpenVanilla::OVIMBig5Code::localizedName(const string &locale) {
    if (locale.find("zh") == 0) {
        return string("Big5 內碼");
    }

    return string("Big5 Code");
}

bool OpenVanilla::OVIMBig5Code::initialize(OVPathInfo *pathInfo, OVLoaderService *loaderService) {
    return true;
}

void OpenVanilla::OVIMBig5Code::loadConfig(OVKeyValueMap *moduleConfig, OVLoaderService *loaderService) {

}

void OpenVanilla::OVIMBig5Code::saveConfig(OVKeyValueMap *moduleConfig, OVLoaderService *loaderService) {

}

