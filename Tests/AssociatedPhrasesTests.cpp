#include "OVAFAssociatedPhrases.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
using namespace OpenVanilla;
class TestModule : public OVAFAssociatedPhrases {
public:
    using OVAFAssociatedPhrases::OVAFAssociatedPhrases;
    vector<string> candidates(const string& key) { return m_table->findChardef(key); }
};
int main(int argc, char **argv) {
    assert(argc == 2 || argc == 3);
    string prefix = "%gen_inp\n%cname Test\n%keyname begin\n%keyname end\n%chardef begin\n";
    string valid = prefix + "一 直\n一 些\n一 時\n%chardef end\n";
    assert(OVAFAssociatedPhrases::ValidateTable(valid));
    assert(!OVAFAssociatedPhrases::ValidateTable(""));
    assert(!OVAFAssociatedPhrases::ValidateTable(prefix + "%chardef end\n"));
    assert(!OVAFAssociatedPhrases::ValidateTable(prefix + "一 直\n"));
    assert(!OVAFAssociatedPhrases::ValidateTable(prefix + "一\n%chardef end\n"));
    assert(!OVAFAssociatedPhrases::ValidateTable(prefix + "一二 三\n%chardef end\n"));
    assert(!OVAFAssociatedPhrases::ValidateTable(prefix + "一 二 三\n%chardef end\n"));
    assert(!OVAFAssociatedPhrases::ValidateTable(valid + "一 外\n"));
    assert(!OVAFAssociatedPhrases::ValidateTable(valid + string(1, '\0')));
    string crlf;
    for (char c : valid) { if (c == '\n') crlf += '\r'; crlf += c; }
    assert(OVAFAssociatedPhrases::ValidateTable(crlf));
    for (int i = 1; i < argc; ++i) {
        std::ifstream input(argv[i]);
        std::ostringstream data; data << input.rdbuf();
        assert(OVAFAssociatedPhrases::ValidateTable(data.str()));
    }
    TestModule module(argv[1]);
    module.setTablePath(argv[1]);
    const auto original = module.candidates("一");
    char temporaryPath[] = "/tmp/openvanilla-phrases-XXXXXX";
    int fd = mkstemp(temporaryPath);
    assert(fd >= 0);
    close(fd);
    { std::ofstream output(temporaryPath); output << valid; }
    module.setTablePath(temporaryPath);
    const auto windows = module.candidates("一");
    assert(windows.size() == 3 && windows[0] == "直" && windows[1] == "些" && windows[2] == "時");
    assert(windows != original);
    module.setTablePath(argv[1]);
    assert(module.candidates("一") == original);
    unlink(temporaryPath);
    std::cout << "PASS: format validation, CRLF, both full tables, Windows order and table switching\n";
}
