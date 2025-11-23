// Copyright (c) 2025 and onwards The OpenVanilla Authors
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

import InfoCollector
import ModuleManager
import OpenVanillaImpl

extension Bool {
    fileprivate var yesNo: String { self ? "Y" : "N" }
}

private class OpenVanillaPreferenceCollector {
    static func generate() -> String {
        var report = "- OpenVanilla Input Methods\n"
        let tables = OVModuleManager.default.inputMethodIdentifiers
        for table in tables {
            report += "  - \(table)\n"
            let layout = OVModuleManager.default.alphanumericKeyboardLayout(
                forInputMethod: table)
            report +=
                OVModuleManager.default.isCustomTableBasedInputMethod(table)
                ? "    - Custom Input Method\n" : "    - Built-in Input Method\n"
            report += "    - Alphanumeric Keyboard: \(layout)\n"
            if table.hasPrefix("org.openvanilla.OVIMTableBased") {
                report +=
                    "    - Clear reading at composition error: "
                    + (PreferencesHelper
                        .boolValue(
                            moduleIdentifier: table, forKey: "ClearReadingBufferAtCompositionError"
                        )
                        .yesNo) + "\n"
                report +=
                    "    - Compose while typing: "
                    + (PreferencesHelper
                        .boolValue(moduleIdentifier: table, forKey: "ComposeWhileTyping")
                        .yesNo) + "\n"
                report +=
                    "    - Should compose at maximum radical length: "
                    + (PreferencesHelper
                        .boolValue(
                            moduleIdentifier: table, forKey: "ShouldComposeAtMaximumRadicalLength")
                        ? "Y" : "N") + "\n"
                report +=
                    "    - Special code prompt: "
                    + (PreferencesHelper
                        .boolValue(moduleIdentifier: table, forKey: "SpecialCodePrompt")
                        ? "Y" : "N") + "\n"
                report +=
                    "    - Use space as first candidate selection key: \(PreferencesHelper.unsignedIntegerValue(moduleIdentifier: table, forKey: "UseSpaceAsFirstCandidateSelectionKey") ?? 0)\n"
                report +=
                    "    - Maximum radical length: \(PreferencesHelper.unsignedIntegerValue(moduleIdentifier: table, forKey: "MaximumRadicalLength") ?? 0)\n"
            } else if table == "org.openvanilla.OVIMArray" {
                report +=
                    "    - Special code prompt: "
                    + (PreferencesHelper
                        .boolValue(moduleIdentifier: table, forKey: "SpecialCodePrompt")
                        .yesNo) + "\n"
                report +=
                    "    - Quick mode enabled: "
                    + (PreferencesHelper
                        .boolValue(moduleIdentifier: table, forKey: "QuickMode")
                        .yesNo) + "\n"

            }
        }
        report +=
            "- Shared Alphanumeric Keyboard Layout: "
            + OVModuleManager.default.sharedAlphanumericKeyboardLayoutIdentifier + "\n"
        report +=
            "- Traditional to Simpified Chinese Filter Enabled: "
            + (OVModuleManager.default.traditionalToSimplifiedChineseFilterEnabled.yesNo)
            + "\n"
        report +=
            "- Simplified to Traditional Chinese Filter Enabled: "
            + (OVModuleManager.default.simplifiedToTraditionalChineseFilterEnabled.yesNo)
            + "\n"
        report +=
            "- Associated Phrase Around Filter Enabled: "
            + (OVModuleManager.default.associatedPhrasesAroundFilterEnabled.yesNo) + "\n"

        let userDefaults = UserDefaults.standard
        report +=
            "- Candidate List Text Size: \(userDefaults.integer(forKey: OVCandidateListTextSizeKey))\n"
        report +=
            "- Candidate List Style Name: \(userDefaults.string(forKey: OVCandidateListStyleNameKey) ?? "Not Set")\n"
        report +=
            "- Fallback to Alphanumeric Keyboard Layout on Shift: \(userDefaults.bool(forKey: OVFallbackToAlphanumericKeyboardLayoutOnShiftKey).yesNo)"
            + "\n"
        report +=
            "- Make Sound Feedback on Input Error: \(userDefaults.bool(forKey: OVMakeSoundFeedbackOnInputErrorKey) ? "Y" : "N")"
            + "\n"
        return report
    }
}

class ReportGenerator {
    static func generate() async -> String {
        var report = ""
        report += await InfoCollector.generate()
        report += OpenVanillaPreferenceCollector.generate()
        return report
    }

}
