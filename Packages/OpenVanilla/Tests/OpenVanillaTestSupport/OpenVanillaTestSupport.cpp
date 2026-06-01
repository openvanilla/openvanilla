#include "OpenVanillaTestSupport.h"

#include <algorithm>
#include <sstream>
#include <vector>

using namespace OpenVanilla;

namespace {

class TestKeyImpl : public OVKeyImpl {
public:
    TestKeyImpl(unsigned int keyCode, const std::string& receivedString, bool shift, bool capsLock)
        : m_keyCode(keyCode)
        , m_receivedString(receivedString)
        , m_shift(shift)
        , m_capsLock(capsLock)
    {
    }

    bool shouldDelete() const override { return true; }
    OVKeyImpl* copy() override { return new TestKeyImpl(m_keyCode, m_receivedString, m_shift, m_capsLock); }
    const std::string receivedString() const override { return m_receivedString; }
    unsigned int keyCode() const override { return m_keyCode; }
    bool isAltPressed() const override { return false; }
    bool isOptPressed() const override { return false; }
    bool isCtrlPressed() const override { return false; }
    bool isShiftPressed() const override { return m_shift; }
    bool isCommandPressed() const override { return false; }
    bool isNumLockOn() const override { return false; }
    bool isCapsLockOn() const override { return m_capsLock; }
    bool isDirectTextKey() const override { return false; }

private:
    unsigned int m_keyCode;
    std::string m_receivedString;
    bool m_shift;
    bool m_capsLock;
};

class TestTextBuffer : public OVTextBuffer {
public:
    void clear() override
    {
        m_text.clear();
        m_toolTip.clear();
    }

    void setText(const std::string& text) override { m_text = text; }
    void appendText(const std::string& text, bool moveCursor = true) override { m_text += text; }

    void commit() override
    {
        m_committedText += m_text;
        m_text.clear();
    }

    void commitAsTextSegment() override { commit(); }
    void commit(const std::string& text) override { m_committedText += text; }
    void commitAsTextSegment(const std::string& text) override { commit(text); }
    void updateDisplay() override {}
    bool isEmpty() const override { return m_text.empty(); }
    size_t codePointCount() const override { return m_text.size(); }
    const std::string codePointAt(size_t index) const override { return index < m_text.size() ? m_text.substr(index, 1) : std::string(); }
    const std::string composedText() const override { return m_text; }
    const std::string composedCommittedText() const override { return m_committedText; }
    const std::vector<std::string> composedCommittedTextSegments() const override { return std::vector<std::string>{m_committedText}; }
    void setCursorPosition(size_t position) override { m_cursorPosition = position; }
    size_t cursorPosition() const override { return m_cursorPosition; }
    void showToolTip(const std::string& text) override { m_toolTip = text; }
    void clearToolTip() override { m_toolTip.clear(); }
    void setHighlightMark(const OVTextBuffer::RangePair& range) override {}
    void setWordSegments(const std::vector<OVTextBuffer::RangePair>& segments) override {}
    void setSuggestedReadingTextStyle(ReadingTextStyle style) override { m_readingTextStyle = style; }
    ReadingTextStyle defaultReadingTextStyle() const override { return m_readingTextStyle; }

    const std::string toolTipText() const { return m_toolTip; }

private:
    std::string m_text;
    std::string m_committedText;
    std::string m_toolTip;
    size_t m_cursorPosition = 0;
    ReadingTextStyle m_readingTextStyle = Horizontal;
};

class TestCandidateList : public OVCandidateList {
public:
    void clear() override { m_candidates.clear(); }
    size_t size() const override { return m_candidates.size(); }
    std::string candidateAtIndex(size_t index) const override { return index < m_candidates.size() ? m_candidates[index] : std::string(); }
    void setCandidate(size_t index, const std::string& candidate) override
    {
        if (index >= m_candidates.size()) {
            m_candidates.resize(index + 1);
        }
        m_candidates[index] = candidate;
    }
    void setCandidates(const std::vector<std::string>& candidates) override { m_candidates = candidates; }
    void addCandidate(const std::string& candidate) override { m_candidates.push_back(candidate); }
    void addCandidates(const std::vector<std::string>& candidates) override
    {
        m_candidates.insert(m_candidates.end(), candidates.begin(), candidates.end());
    }

private:
    std::vector<std::string> m_candidates;
};

class TestOneDimensionalCandidatePanel : public OVOneDimensionalCandidatePanel {
public:
    void hide() override { m_visible = false; }
    void show() override { m_visible = true; }
    void updateDisplay() override {}
    bool isVisible() override { return m_visible; }
    void setPrompt(const std::string& prompt) override { m_prompt = prompt; }
    std::string prompt() override { return m_prompt; }
    bool yieldToCandidateEventHandler() override { return false; }
    void cancelEventHandler() override {}
    void reset() override
    {
        m_candidateList.clear();
        m_visible = false;
        m_currentPage = 0;
    }

    bool isHorizontal() const override { return false; }
    bool isVertical() const override { return true; }
    OVCandidateList* candidateList() override { return &m_candidateList; }
    size_t candidatesPerPage() const override { return m_candidatesPerPage; }
    void setCandidatesPerPage(size_t number) override { m_candidatesPerPage = std::max<size_t>(number, 1); }
    size_t pageCount() const override
    {
        size_t size = m_candidateList.size();
        return size ? ((size + m_candidatesPerPage - 1) / m_candidatesPerPage) : 0;
    }
    size_t currentPage() const override { return m_currentPage; }
    size_t currentPageCandidateCount() const override
    {
        if (!pageCount()) {
            return 0;
        }
        size_t remaining = m_candidateList.size() - (m_currentPage * m_candidatesPerPage);
        return std::min(remaining, m_candidatesPerPage);
    }
    bool allowsPageWrapping() const override { return m_allowsPageWrapping; }
    void setAllowsPageWrapping(bool allowsPageWrapping) override { m_allowsPageWrapping = allowsPageWrapping; }
    size_t currentHightlightIndex() const override { return m_highlightIndex; }
    void setHighlightIndex(size_t index) override { m_highlightIndex = index; }
    size_t currentHighlightIndexInCandidateList() const override { return m_currentPage * m_candidatesPerPage + m_highlightIndex; }

    size_t goToNextPage() override
    {
        if (m_currentPage + 1 < pageCount()) {
            ++m_currentPage;
        }
        else if (m_allowsPageWrapping) {
            m_currentPage = 0;
        }
        return m_currentPage;
    }

    size_t goToPreviousPage() override
    {
        if (m_currentPage > 0) {
            --m_currentPage;
        }
        else if (m_allowsPageWrapping && pageCount()) {
            m_currentPage = pageCount() - 1;
        }
        return m_currentPage;
    }

    size_t goToPage(size_t page) override
    {
        if (page < pageCount()) {
            m_currentPage = page;
        }
        return m_currentPage;
    }

    const OVKey candidateKeyAtIndex(size_t index) override { return index < m_candidateKeys.size() ? m_candidateKeys[index] : OVKey(); }
    void setCandidateKeys(const OVKeyVector& keys) override { m_candidateKeys = keys; }
    void setNextPageKeys(const OVKeyVector& keys) override {}
    void setPreviousPageKeys(const OVKeyVector& keys) override {}
    void setNextCandidateKeys(const OVKeyVector& keys) override {}
    void setPreviousCandidateKeys(const OVKeyVector& keys) override {}
    void setCancelKeys(const OVKeyVector& keys) override {}
    void setChooseHighlightedCandidateKeys(const OVKeyVector& keys) override {}
    const OVKeyVector defaultCandidateKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultNextPageKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultNextCandidateKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultPreviousPageKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultPreviousCandidateKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultCancelKeys() const override { return OVKeyVector(); }
    const OVKeyVector defaultChooseHighlightedCandidateKeys() const override { return OVKeyVector(); }

    void setCandidateKeysAndLabels(const std::vector<std::pair<OVKey, std::string>>& keyLabelPairs) override
    {
        m_candidateKeys.clear();
        for (std::vector<std::pair<OVKey, std::string>>::const_iterator iter = keyLabelPairs.begin(); iter != keyLabelPairs.end(); ++iter) {
            m_candidateKeys.push_back(iter->first);
        }
        setCandidatesPerPage(keyLabelPairs.size());
    }

private:
    TestCandidateList m_candidateList;
    bool m_visible = false;
    bool m_allowsPageWrapping = false;
    size_t m_candidatesPerPage = 9;
    size_t m_currentPage = 0;
    size_t m_highlightIndex = 0;
    std::string m_prompt;
    OVKeyVector m_candidateKeys;
};

class TestCandidateService : public OVCandidateService {
public:
    OVOneDimensionalCandidatePanel* useVerticalCandidatePanel() override { return &m_panel; }
    OVOneDimensionalCandidatePanel* useOneDimensionalCandidatePanel() override { return &m_panel; }
    TestOneDimensionalCandidatePanel* panel() { return &m_panel; }

private:
    TestOneDimensionalCandidatePanel m_panel;
};

class TestLoaderService : public OVLoaderService {
public:
    void beep() override { ++m_beepCount; }
    void notify(const std::string& message) override {}
    void HTMLNotify(const std::string& content) override {}
    const std::string locale() const override { return "en"; }
    const OVKey makeOVKey(int characterCode, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false) override
    {
        std::string receivedString;
        if (characterCode >= 32 && characterCode <= 126) {
            receivedString = std::string(1, static_cast<char>(characterCode));
        }
        return OVKey(new TestKeyImpl(static_cast<unsigned int>(characterCode), receivedString, shift, capsLock));
    }
    const OVKey makeOVKey(const std::string& receivedString, bool alt = false, bool opt = false, bool ctrl = false, bool shift = false, bool command = false, bool capsLock = false, bool numLock = false) override
    {
        unsigned int keyCode = receivedString.size() == 1 ? static_cast<unsigned int>(receivedString[0]) : 0;
        return OVKey(new TestKeyImpl(keyCode, receivedString, shift, capsLock));
    }
    std::ostream& logger(const std::string& sectionName = "") override { return m_logStream; }
    OVDatabaseService* defaultDatabaseService() override { return 0; }
    OVDatabaseService* CINDatabaseService() override { return 0; }
    OVDatabaseService* SQLiteDatabaseService() override { return 0; }
    OVEncodingService* encodingService() override { return 0; }
    void __reserved1(const std::string&) override {}
    void __reserved2(const std::string&) override {}
    void __reserved3(const std::string&) override {}
    void __reserved4(const std::string&) override {}
    const std::string __reserved5() const override { return std::string(); }
    void __reserved6(const std::string&) override {}
    void __reserved7(const std::string&, const std::string&) override {}
    void* __reserved8(const std::string&) override { return 0; }
    int beepCount() const { return m_beepCount; }

private:
    int m_beepCount = 0;
    std::ostringstream m_logStream;
};

}

namespace OpenVanillaTestSupport {

class OVIMArrayContextTestHarness::Impl {
public:
    Impl(const std::string& tableRootPath)
        : module(tableRootPath)
    {
        if (module.checkTable()) {
            context = module.createContext();
        }
    }

    ~Impl()
    {
        delete context;
    }

    OVIMArray module;
    OVEventHandlingContext* context = 0;
    TestTextBuffer readingText;
    TestTextBuffer composingText;
    TestCandidateService candidateService;
    TestLoaderService loaderService;
    int referenceCount = 1;
};

OVIMArrayContextTestHarness::OVIMArrayContextTestHarness(const std::string& tableRootPath)
    : m_impl(new Impl(tableRootPath))
{
}

OVIMArrayContextTestHarness::OVIMArrayContextTestHarness(const OVIMArrayContextTestHarness& harness)
    : m_impl(harness.m_impl)
{
    ++m_impl->referenceCount;
}

OVIMArrayContextTestHarness& OVIMArrayContextTestHarness::operator=(const OVIMArrayContextTestHarness& harness)
{
    if (this == &harness) {
        return *this;
    }

    if (--m_impl->referenceCount == 0) {
        delete m_impl;
    }

    m_impl = harness.m_impl;
    ++m_impl->referenceCount;
    return *this;
}

OVIMArrayContextTestHarness::~OVIMArrayContextTestHarness()
{
    if (--m_impl->referenceCount == 0) {
        delete m_impl;
    }
}

bool OVIMArrayContextTestHarness::isReady() const
{
    return m_impl->context != 0;
}

bool OVIMArrayContextTestHarness::handleKey(int keyCode)
{
    return handleKey(keyCode, false, false);
}

bool OVIMArrayContextTestHarness::handleKey(int keyCode, bool shift, bool capsLock)
{
    if (!m_impl->context) {
        return false;
    }

    OVKey key = m_impl->loaderService.makeOVKey(keyCode, false, false, false, shift, false, capsLock, false);
    return m_impl->context->handleKey(&key, &m_impl->readingText, &m_impl->composingText, &m_impl->candidateService, &m_impl->loaderService);
}

const std::string OVIMArrayContextTestHarness::readingText() const
{
    return m_impl->readingText.composedText();
}

const std::string OVIMArrayContextTestHarness::composingText() const
{
    return m_impl->composingText.composedText();
}

const std::string OVIMArrayContextTestHarness::committedText() const
{
    return m_impl->composingText.composedCommittedText();
}

const std::string OVIMArrayContextTestHarness::toolTipText() const
{
    return m_impl->composingText.toolTipText();
}

bool OVIMArrayContextTestHarness::candidatePanelVisible() const
{
    return m_impl->candidateService.panel()->isVisible();
}

size_t OVIMArrayContextTestHarness::candidateCount() const
{
    return m_impl->candidateService.panel()->candidateList()->size();
}

const std::string OVIMArrayContextTestHarness::candidateAt(size_t index) const
{
    return m_impl->candidateService.panel()->candidateList()->candidateAtIndex(index);
}

int OVIMArrayContextTestHarness::beepCount() const
{
    return m_impl->loaderService.beepCount();
}

bool OVIMArrayContextTestHarness::isForceSP() const
{
    return m_impl->module.isForceSP();
}

}
