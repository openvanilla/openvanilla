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
import InputSourceHelper

private let kTargetBin = "OpenVanilla"
private let kTargetType = "app"
private let kTargetBundle = "OpenVanilla.app"
private let kDestinationPartial = "~/Library/Input Methods/"
private let kTargetPartialPath = "~/Library/Input Methods/OpenVanilla.app"
private let kTargetFullBinPartialPath = "~/Library/Input Methods/OpenVanilla.app/Contents/MacOS/OpenVanilla"

private let kTranslocationRemovalTickInterval: TimeInterval = 0.5
private let kTranslocationRemovalDeadline: TimeInterval = 60.0

@NSApplicationMain
@objc(AppDelegate)
class AppDelegate: NSWindowController, NSApplicationDelegate {
    @IBOutlet weak private var actionButton: NSButton!
    @IBOutlet weak private var cancelButton: NSButton!
    @IBOutlet private var textView: NSTextView!
    @IBOutlet weak private var progressSheet: NSWindow!
    @IBOutlet weak private var progressIndicator: NSProgressIndicator!
    @IBOutlet weak var welcomeText: NSTextField!
    @IBOutlet weak var agreeText: NSTextField!
    
    private var archiveUtil: ArchiveUtil?
    private var installingVersion = ""
    private var upgrading = false
    private var translocationRemovalStartTime: Date?
    private var currentVersionNumber: Int = 0

    private var installed = false

    func runAlertPanel(title: String, message: String, buttonTitle: String) {
        let alert = NSAlert()
        alert.alertStyle = .informational
        alert.messageText = title
        alert.informativeText = message
        alert.addButton(withTitle: buttonTitle)
        alert.runModal()
    }

    func applicationDidFinishLaunching(_ notification: Notification) {
        guard let installingVersion = Bundle.main.infoDictionary?[kCFBundleVersionKey as String] as? String,
              let versionString = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String else {
            return
        }
        self.installingVersion = installingVersion
        self.archiveUtil = ArchiveUtil(appName: kTargetBin, targetAppBundleName: kTargetBundle)
        _ = archiveUtil?.validateIfNotarizedArchiveExists()

        cancelButton.nextKeyView = actionButton
        actionButton.nextKeyView = cancelButton
        if let cell = actionButton.cell as? NSButtonCell {
            window?.defaultButtonCell = cell
        }

        var attrStr = NSAttributedString()
        if let rtfPath = Bundle.main.url(forResource: "License", withExtension: "rtf"),
           let rtfData = try? Data(contentsOf: rtfPath),
           let rtf = NSAttributedString(rtf: rtfData, documentAttributes: nil) {
            attrStr = rtf
        }

        let mutableAttrStr = NSMutableAttributedString(attributedString: attrStr)
        mutableAttrStr.addAttribute(.foregroundColor, value: NSColor.controlTextColor, range: NSMakeRange(0, mutableAttrStr.length))
        textView.textStorage?.setAttributedString(mutableAttrStr)
        textView.setSelectedRange(NSMakeRange(0, 0))
        window?.title = String(format: NSLocalizedString("%@ (for version %@, r%@)", comment: ""), window?.title ?? "", versionString, installingVersion)

        if FileManager.default.fileExists(atPath: (kTargetPartialPath as NSString).expandingTildeInPath) {
            let currentBundle = Bundle(path: (kTargetPartialPath as NSString).expandingTildeInPath)
            let shortVersion = currentBundle?.infoDictionary?["CFBundleShortVersionString"] as? String
            let currentVersion = currentBundle?.infoDictionary?[kCFBundleVersionKey as String] as? String
            currentVersionNumber = (currentVersion as NSString?)?.integerValue ?? 0
            if shortVersion != nil, let currentVersion = currentVersion, currentVersion.compare(installingVersion, options: .numeric) == .orderedAscending {
                upgrading = true
            }
        }

        if upgrading {
            actionButton.title = NSLocalizedString("Agree and Upgrade", comment: "")
        }

        window?.center()
        window?.orderFront(self)
        NSApp.activate(ignoringOtherApps: true)
    }

    @IBAction func agreeAndInstallAction(_ sender: AnyObject) {
        if !installed {
            // user agreed to terms for install
            cancelButton.isEnabled = false
            actionButton.isEnabled = false
            removeThenInstallInputMethod()
        } else {
            // installation was successful, user clicked the "Close Installer" button
            endAppWithDelay()
        }
    }

    @objc func timerTick(_ timer: Timer) {
        let elapsed = Date().timeIntervalSince(translocationRemovalStartTime ?? Date())
        if elapsed >= kTranslocationRemovalDeadline {
            timer.invalidate()
            window?.endSheet(progressSheet, returnCode: .cancel)
        } else if appBundleTranslocatedToARandomizedPath(kTargetPartialPath) == false {
            progressIndicator.doubleValue = 1.0
            timer.invalidate()
            window?.endSheet(progressSheet, returnCode: .continue)
        }
    }

    func removeThenInstallInputMethod() {
        if FileManager.default.fileExists(atPath: (kTargetPartialPath as NSString).expandingTildeInPath) == false {
            self.installInputMethod(previousExists: false, previousVersionNotFullyDeactivatedWarning: false)
            return
        }

        let shouldWaitForTranslocationRemoval = appBundleTranslocatedToARandomizedPath(kTargetPartialPath) && (window?.responds(to: #selector(NSWindow.beginSheet(_:completionHandler:))) ?? false)

        let fullDir = (kDestinationPartial as NSString).expandingTildeInPath
        let targetURL = NSURL.fileURL(withPath: fullDir).appendingPathComponent(kTargetBundle)
        NSWorkspace.shared.recycle([targetURL]) { [self] (urls: [URL : URL], error: Error?) in
            if let error = error {
                let message = String(format: NSLocalizedString("Cannot remove the existing version at %@, error: %@", comment: ""), targetURL.path, String(describing: error))
                runAlertPanel(title: NSLocalizedString("Install Failed", comment: ""),
                              message: message,
                              buttonTitle: NSLocalizedString("Cancel", comment: ""))

                endAppWithDelay()
                return
            }

            let killTask = Process()
            killTask.launchPath = "/usr/bin/killall"
            killTask.arguments = ["-9", kTargetBin]
            killTask.launch()
            killTask.waitUntilExit()

            if shouldWaitForTranslocationRemoval {
                progressIndicator.startAnimation(self)
                window?.beginSheet(progressSheet) { returnCode in
                    DispatchQueue.main.async {
                        if returnCode == .continue {
                            self.installInputMethod(previousExists: true, previousVersionNotFullyDeactivatedWarning: false)
                        } else {
                            self.installInputMethod(previousExists: true, previousVersionNotFullyDeactivatedWarning: true)
                        }
                    }
                }

                translocationRemovalStartTime = Date()
                Timer.scheduledTimer(timeInterval: kTranslocationRemovalTickInterval, target: self, selector: #selector(timerTick(_:)), userInfo: nil, repeats: true)
            } else {
                self.installInputMethod(previousExists: false, previousVersionNotFullyDeactivatedWarning: false)
            }
        }
    }

    func installInputMethod(previousExists: Bool, previousVersionNotFullyDeactivatedWarning warning: Bool) {
        guard let targetBundle = archiveUtil?.unzipNotarizedArchive() ?? Bundle.main.path(forResource: kTargetBin, ofType: kTargetType) else {
            let message = NSLocalizedString("No installable packages found.", comment: "")
            runAlertPanel(title: NSLocalizedString("Fatal Error", comment: ""), message: message, buttonTitle: NSLocalizedString("Abort", comment: ""))
            endAppWithDelay()
            return
        }
        let installedPath = (kTargetPartialPath as NSString).expandingTildeInPath

        let cpTask = Process()
        cpTask.launchPath = "/bin/cp"
        cpTask.arguments = ["-R", targetBundle, (kDestinationPartial as NSString).expandingTildeInPath]
        cpTask.launch()
        cpTask.waitUntilExit()

        if cpTask.terminationStatus != 0 {
            runAlertPanel(title: NSLocalizedString("Install Failed", comment: ""),
                          message: NSLocalizedString("Cannot copy the file to the destination.", comment: ""),
                          buttonTitle: NSLocalizedString("Cancel", comment: ""))
            endAppWithDelay()
            return
        }

        // Strip quarantine / Gatekeeper xattrs so macOS does not App-Translocate the IME.
        let xattrTask = Process()
        xattrTask.launchPath = "/usr/bin/xattr"
        xattrTask.arguments = ["-cr", installedPath]
        xattrTask.launch()
        xattrTask.waitUntilExit()
        if xattrTask.terminationStatus != 0 {
            NSLog("Warning: xattr -cr failed on \(installedPath) with status \(xattrTask.terminationStatus)")
        }

        // Force Launch Services to index the real install path (not Trash / AppTranslocation).
        let lsregister = "/System/Library/Frameworks/CoreServices.framework/Frameworks/LaunchServices.framework/Support/lsregister"
        if FileManager.default.isExecutableFile(atPath: lsregister) {
            let lsTask = Process()
            lsTask.launchPath = lsregister
            lsTask.arguments = ["-f", installedPath]
            lsTask.launch()
            lsTask.waitUntilExit()
        }

        guard let imeBundle = Bundle(path: installedPath),
              let imeIdentifier = imeBundle.bundleIdentifier
                else {
            endAppWithDelay()
            return
        }

        let imeBundleURL = imeBundle.bundleURL

        // Always re-register at the installed path so stale Trash/AppTranslocation registrations
        // are overwritten after upgrades.
        NSLog("Registering input source \(imeIdentifier) at \(imeBundleURL.absoluteString).")
        let registered = InputSourceHelper.registerInputSource(at: imeBundleURL)
        if !registered {
            NSLog("Warning: TISRegisterInputSource returned false for \(imeBundleURL.absoluteString)")
        }

        guard let inputSource = InputSourceHelper.inputSource(for: imeIdentifier) else {
            let message = String(format: NSLocalizedString("Cannot find input source %@ after registration.", comment: ""), imeIdentifier)
            runAlertPanel(title: NSLocalizedString("Fatal Error", comment: ""), message: message, buttonTitle: NSLocalizedString("Abort", comment: ""))
            endAppWithDelay()
            return
        }

        // Always attempt enable. On macOS 12+, TIS may return success while IsEnabled stays
        // false until the user confirms in System Settings — check the actual flag.
        if #available(macOS 12.0, *) {
            NSLog("macOS 12 or later detected.")
        } else {
            NSLog("Installer runs with the pre-macOS 12 flow.")
        }
        _ = InputSourceHelper.enable(inputSource: inputSource)
        let mainInputSourceEnabled = InputSourceHelper.inputSourceEnabled(for: inputSource)
        if mainInputSourceEnabled {
            NSLog("Input method enabled: \(imeIdentifier)")
        } else {
            NSLog("Failed to enable input method: \(imeIdentifier)")
        }

        if warning {
            runAlertPanel(title: NSLocalizedString("Attention", comment: ""), message: NSLocalizedString("OpenVanilla is upgraded, but please log out or reboot for the new version to be fully functional.", comment: ""), buttonTitle: NSLocalizedString("OK", comment: ""))
            endAppWithDelay()
        } else {
            if !mainInputSourceEnabled {
                runAlertPanel(
                    title: NSLocalizedString("Warning", comment: ""),
                    message: NSLocalizedString(
                        "Input method was installed, but could not be enabled automatically. Please add OpenVanilla in System Settings > Keyboard > Input Sources, then log out and log back in.",
                        comment: ""),
                    buttonTitle: NSLocalizedString("Continue", comment: ""))
            }

            let headlineAttr = [
                NSAttributedString.Key.font : NSFont.boldSystemFont(ofSize: NSFont.systemFontSize * 1.3),
                NSAttributedString.Key.foregroundColor : NSColor.textColor
            ]
            let bodyAttr = [
                NSAttributedString.Key.font : NSFont.systemFont(ofSize: NSFont.systemFontSize),
                NSAttributedString.Key.foregroundColor : NSColor.textColor
            ]
            let message = NSMutableAttributedString(string: NSLocalizedString("Installation Successful", comment: ""), attributes: headlineAttr)
            let detailsKey =
                mainInputSourceEnabled
                ? "OpenVanilla is ready to use."
                : "Input method was installed, but could not be enabled automatically. Please add OpenVanilla in System Settings > Keyboard > Input Sources, then log out and log back in."
            let details = NSAttributedString(string: NSLocalizedString(detailsKey, comment: ""), attributes: bodyAttr)
            message.append(NSAttributedString(string: "\n\n"))
            message.append(details)
            textView.textStorage?.setAttributedString(message)

            installed = true

            welcomeText.isHidden = true
            cancelButton.isHidden = true
            actionButton.title = NSLocalizedString("Close Installer", comment: "")
            actionButton.isEnabled = true

            // Only auto-close when enable looked successful; otherwise leave the guidance visible.
            if mainInputSourceEnabled {
                scheduleAutoClose()
            }
        }
    }

    func endAppWithDelay() {
        DispatchQueue.main.asyncAfter(deadline: DispatchTime.now() + 0.1) {
            NSApp.terminate(self)
        }
    }

    @IBAction func cancelAction(_ sender: AnyObject) {
        NSApp.terminate(self)
    }

    func windowWillClose(_ Notification: Notification) {
        NSApp.terminate(self)
    }

    func updateAutoClosingMessage(seconds: Int) {
        agreeText.stringValue = String.localizedStringWithFormat(
            NSLocalizedString("The installer will close itself in %d second(s).", comment: ""),
            seconds)
    }

    func scheduleAutoClose() {
        var counter = 60
        updateAutoClosingMessage(seconds: counter)

        Timer.scheduledTimer(withTimeInterval: 1.0, repeats: true) { [weak self] timer in
            counter -= 1
            if counter == 0 {
                timer.invalidate()
                self?.endAppWithDelay()
            } else {
                self?.updateAutoClosingMessage(seconds: counter)
            }
        }
    }
}
