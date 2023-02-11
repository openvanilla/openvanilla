
#ifndef OVIMBig5CodeContext_h
#define OVIMBig5CodeContext_h

#include "OpenVanilla.h"
#include "OVIMBig5Code.h"

namespace OpenVanilla {
    using namespace std;

    class OVIMBig5CodeContext : public OVEventHandlingContext
    {
    public:
        OVIMBig5CodeContext(OVIMBig5Code *module);

        virtual void startSession(OVLoaderService *loaderService);

        virtual bool handleKey(OVKey *key, OVTextBuffer *readingText, OVTextBuffer *composingText, OVCandidateService *candidateService, OVLoaderService *loaderService);

        virtual void candidateCanceled(OVCandidateService *candidateService, OVTextBuffer *readingText, OVTextBuffer *composingText, OVLoaderService *loaderService);

        virtual bool candidateSelected(OVCandidateService *candidateService, const string &text, size_t index, OVTextBuffer *readingText, OVTextBuffer *composingText, OVLoaderService *loaderService);

        virtual bool candidateNonPanelKeyReceived(OVCandidateService *candidateService, const OVKey *key, OVTextBuffer *readingText, OVTextBuffer *composingText, OVLoaderService *loaderService);

    protected:
        OVIMBig5Code *m_module;
        string m_code;
    };

}


#endif

