// Run with Tests/run-table-based-width.sh from the source directory.
#include "OVIMTableBased.h"
#include "OVIMTableBasedContext.h"
#include "OVConcreteKeyImpl.h"
#include "OVTextBufferImpl.h"
#include <cassert>
using namespace OpenVanilla;
struct Config : OVKeyValueMap {
    bool enabled = true;
    map<string, bool> saved;
    bool setKeyBoolValue(const string& key, bool value) override { saved[key] = value; return true; }
    bool hasKey(const string& key) override { return key == "ShiftSpaceTogglesWidth"; }
    bool isKeyTrue(const string& key) override { return hasKey(key) && enabled; }
};
int main() {
    for (const string name : {"cj", "simplex"}) {
        OVIMTableBased module("Source/Mac/MacDataTables/" + name + ".cin");
        Config config;
        module.loadConfig(&config, nullptr);
        module.saveConfig(&config, nullptr);
        assert(config.saved["ShiftSpaceTogglesWidth"]);
        OVIMTableBasedContext context(&module);
        context.startSession(nullptr);
        OVTextBufferImpl reading, composing;
        auto press = [&](char c, bool shift = false, bool command = false, bool caps = false, bool num = false) {
            composing.finishCommit();
            OVKey key(new OVConcreteKeyImpl(c, false, false, false, shift, command, caps, num));
            return context.handleKey(&key, &reading, &composing, nullptr, nullptr);
        };
        for (char c = '0'; c <= '9'; ++c) {
            assert(press(c));
            assert(composing.composedCommittedText() == OVUTF8Helper::SingleUTF8StringFromCodePoint(c + 0xfee0));
        }
        assert(press(' ', true) && !composing.isCommitted());
        for (char c = '!'; c <= '~'; ++c) {
            if (isalpha(c)) continue;
            assert(press(c));
            assert(composing.composedCommittedText() == string(1, c));
        }
        assert(press(' ', true) && !composing.isCommitted());
        assert(press('9', false, false, false, true));
        assert(composing.composedCommittedText() == "９");
        assert(!press(' ', true, true)); // Command-Shift-Space is not reserved.
        assert(press('2') && composing.composedCommittedText() == "２");
        assert(press('3', false, false, true));
        assert(composing.composedCommittedText() == "3");
        assert(press('a') && !reading.isEmpty());
        auto before = reading.composedText();
        assert(press(' ', true) && reading.composedText() == before);
        OVKey toggle(new OVConcreteKeyImpl(' ', false, false, false, true, false, false, false));
        assert(context.candidateNonPanelKeyReceived(nullptr, &toggle, &reading, &composing, nullptr));
        assert(reading.composedText() == before && !composing.isCommitted());
        reading.clear();
        context.startSession(nullptr);
        assert(press('4') && composing.composedCommittedText() == "４");
        config.enabled = false;
        module.loadConfig(&config, nullptr);
        assert(press(' ', true) && composing.composedCommittedText() == " ");
        assert(press('5') && composing.composedCommittedText() == "5");
    }
    OVIMTableBased other("dayi3.cin");
    Config config;
    other.loadConfig(&config, nullptr);
    assert(!other.shiftSpaceTogglesWidth());
    OVIMTableBased defaults("cj.cin");
    assert(!defaults.shiftSpaceTogglesWidth());
    std::cout << "PASS: width toggle, ASCII digits/symbols, numpad, modifiers, composition, candidate shortcut, defaults and scope\n";
}
