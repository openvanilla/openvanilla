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

private let kExpectedInputMethodPath = (NSString(string: "~/Library/Input Methods/OpenVanilla.app")
    .expandingTildeInPath)

private func pathLooksTranslocated(_ path: String) -> Bool {
    path.contains("AppTranslocation")
}

private func pathHasQuarantine(_ path: String) -> Bool {
    let task = Process()
    task.executableURL = URL(fileURLWithPath: "/usr/bin/xattr")
    task.arguments = ["-lr", path]
    let pipe = Pipe()
    task.standardOutput = pipe
    task.standardError = FileHandle.nullDevice
    do {
        try task.run()
        task.waitUntilExit()
    } catch {
        return false
    }
    let data = pipe.fileHandleForReading.readDataToEndOfFile()
    let output = String(data: data, encoding: .utf8) ?? ""
    return output.contains("com.apple.quarantine")
}

@discardableResult
private func stripQuarantine(at path: String) -> Bool {
    let task = Process()
    task.executableURL = URL(fileURLWithPath: "/usr/bin/xattr")
    task.arguments = ["-cr", path]
    do {
        try task.run()
        task.waitUntilExit()
    } catch {
        NSLog("Warning: xattr -cr failed to start on \(path): \(error)")
        return false
    }
    if task.terminationStatus != 0 {
        NSLog("Warning: xattr -cr failed on \(path) with status \(task.terminationStatus)")
        return false
    }
    return !pathHasQuarantine(path)
}

private func registerInputSource(at path: String) {
    guard Bundle.main.bundleIdentifier != nil else {
        return
    }
    let url = URL(fileURLWithPath: path)
    NSLog("Registering input source at \(url.absoluteString)")
    _ = InputSourceHelper.registerInputSource(at: url)
}

private func canSafelyRelaunch(from installPath: String) -> Bool {
    let expectedExec = (installPath as NSString)
        .appendingPathComponent("Contents/MacOS/OpenVanilla")
    return FileManager.default.isExecutableFile(atPath: expectedExec)
        && !pathLooksTranslocated(installPath)
        && !pathHasQuarantine(installPath)
}

/// If we were launched from App Translocation, try to hand off to the real install.
/// Returns true when this process should exit immediately (never serve IMK from translocation).
private func recoverFromAppTranslocationIfNeeded() -> Bool {
    let bundlePath = Bundle.main.bundlePath
    guard pathLooksTranslocated(bundlePath) else {
        if pathHasQuarantine(bundlePath) {
            NSLog("Warning: quarantine still present on \(bundlePath); attempting to clear.")
            _ = stripQuarantine(at: bundlePath)
        }
        registerInputSource(at: bundlePath)
        return false
    }

    NSLog(
        "Warning: OpenVanilla is running from App Translocation (\(bundlePath)). Attempting to recover via \(kExpectedInputMethodPath)."
    )
    guard FileManager.default.fileExists(atPath: kExpectedInputMethodPath) else {
        // No real install to hand off to — refuse to become IMKServer from a translocated path.
        NSLog(
            "No install at \(kExpectedInputMethodPath). Exiting translocated process; reinstall OpenVanilla."
        )
        return true
    }

    if pathHasQuarantine(kExpectedInputMethodPath) {
        NSLog("Clearing quarantine on \(kExpectedInputMethodPath) before relaunch.")
        _ = stripQuarantine(at: kExpectedInputMethodPath)
    }

    registerInputSource(at: kExpectedInputMethodPath)

    let expectedExec = (kExpectedInputMethodPath as NSString)
        .appendingPathComponent("Contents/MacOS/OpenVanilla")
    guard canSafelyRelaunch(from: kExpectedInputMethodPath) else {
        NSLog(
            "Cannot safely relaunch from \(kExpectedInputMethodPath) (missing, translocated, or still quarantined). Exiting without starting IMK from App Translocation."
        )
        return true
    }

    let relaunch = Process()
    relaunch.executableURL = URL(fileURLWithPath: expectedExec)
    do {
        try relaunch.run()
        NSLog("Relaunched OpenVanilla from \(expectedExec); exiting translocated process.")
        return true
    } catch {
        NSLog("Failed to relaunch from \(expectedExec): \(error). Exiting translocated process.")
        return true
    }
}

private func install() -> Int32 {
    guard let bundleID = Bundle.main.bundleIdentifier else {
        return -1
    }
    let bundleUrl = Bundle.main.bundleURL

    registerInputSource(at: Bundle.main.bundlePath)

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

// Resolve App Translocation before creating IMKServer / loading UI.
if recoverFromAppTranslocationIfNeeded() {
    exit(0)
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

guard let bundleID = Bundle.main.bundleIdentifier,
    let server = IMKServer(name: OVInputMethodConnectionName, bundleIdentifier: bundleID)
else {
    NSLog(
        "Fatal error: Cannot initialize input method server with connection \(OVInputMethodConnectionName)."
    )
    exit(-1)
}

NSApp.run()
