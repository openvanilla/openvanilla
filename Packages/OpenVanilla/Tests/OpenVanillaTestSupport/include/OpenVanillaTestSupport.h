#ifndef OpenVanillaTestSupport_h
#define OpenVanillaTestSupport_h

#include <string>

#include "OVIMArray.h"

namespace OpenVanillaTestSupport {

class OVIMArrayContextTestHarness {
public:
    OVIMArrayContextTestHarness(const std::string& tableRootPath);
    OVIMArrayContextTestHarness(const OVIMArrayContextTestHarness& harness);
    OVIMArrayContextTestHarness& operator=(const OVIMArrayContextTestHarness& harness);
    ~OVIMArrayContextTestHarness();

    bool isReady() const;
    bool handleKey(int keyCode);
    bool handleKey(int keyCode, bool shift, bool capsLock);

    const std::string readingText() const;
    const std::string composingText() const;
    const std::string committedText() const;
    const std::string toolTipText() const;

    bool candidatePanelVisible() const;
    size_t candidateCount() const;
    const std::string candidateAt(size_t index) const;

    int beepCount() const;
    bool isForceSP() const;

private:
    class Impl;
    Impl* m_impl;
};

}

#endif
