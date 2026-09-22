// Copyright (c) 2022 and onwards The OpenVanilla Authors.
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

import Cocoa
import InputMethodKit
import InputSourceHelper
import OpenVanillaImpl

private func registerInputSourceAtBundleURL() {
    guard let bundleID = Bundle.main.bundleIdentifier else {
        return
    }
    let bundleUrl = Bundle.main.bundleURL
    // Always re-register so Launch Services / TIS keep the real bundle path
    // (not a stale Trash or AppTranslocation location).
    NSLog("Registering input source \(bundleID) at \(bundleUrl.absoluteString)")
    _ = InputSourceHelper.registerInputSource(at: bundleUrl)
}

private func install() -> Int32 {
    guard let bundleID = Bundle.main.bundleIdentifier else {
        return -1
    }
    let bundleUrl = Bundle.main.bundleURL

    registerInputSourceAtBundleURL()

    guard let inputSource = InputSourceHelper.inputSource(for: bundleID) else {
        NSLog("Fatal error: Cannot find input source \(bundleID) after registration.")
        return -1
    }

    // On modern macOS, TISEnableInputSource may report failure / leave IsEnabled false
    // until the user confirms in System Settings. Treat that as non-fatal.
    NSLog("Enabling input source \(bundleID) at \(bundleUrl.absoluteString).")
    let enabled = InputSourceHelper.enable(inputSource: inputSource)
    if !enabled || !InputSourceHelper.inputSourceEnabled(for: inputSource) {
        NSLog(
            "Warning: Cannot fully enable input source \(bundleID). User may need to add it in System Settings > Keyboard > Input Sources, then log out."
        )
    }

    if CommandLine.arguments.count > 2 && CommandLine.arguments[2] == "--all" {
        let allEnabled = InputSourceHelper.enableAllInputMode(for: bundleID)
        NSLog(
            allEnabled
                ? "All input sources enabled for \(bundleID)"
                : "Cannot enable all input sources for \(bundleID), but this is ignored")
    }
    return 0
}

if CommandLine.arguments.count > 1 {
    if CommandLine.arguments[1] == "install" {
        let exitCode = install()
        exit(exitCode)
    }
}

guard let mainNibName = Bundle.main.infoDictionary?["NSMainNibFile"] as? String else {
    NSLog("Fatal error: NSMainNibFile key not defined in Info.plist.")
    exit(-1)
}

let loaded = Bundle.main.loadNibNamed(mainNibName, owner: NSApp, topLevelObjects: nil)
if !loaded {
    NSLog("Fatal error: Cannot load \(mainNibName).")
    exit(-1)
}

let bundlePath = Bundle.main.bundlePath
if bundlePath.contains("AppTranslocation") {
    NSLog(
        "Warning: OpenVanilla is running from App Translocation (\(bundlePath)). Quarantine xattrs likely remain; reinstall with a fixed installer or clear quarantine on ~/Library/Input Methods/OpenVanilla.app."
    )
    let expectedPath = (NSString(string: "~/Library/Input Methods/OpenVanilla.app")
        .expandingTildeInPath)
    if FileManager.default.fileExists(atPath: expectedPath) {
        let expectedURL = URL(fileURLWithPath: expectedPath)
        NSLog("Re-registering input source at expected path \(expectedURL.absoluteString)")
        _ = InputSourceHelper.registerInputSource(at: expectedURL)
    }
} else {
    registerInputSourceAtBundleURL()
}

guard let bundleID = Bundle.main.bundleIdentifier,
    let server = IMKServer(name: OVInputMethodConnectionName, bundleIdentifier: bundleID)
else {
    NSLog(
        "Fatal error: Cannot initialize input method server with connection \(OVInputMethodConnectionName)."
    )
    exit(-1)
}

NSApp.run()
