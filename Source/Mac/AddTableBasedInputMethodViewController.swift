// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
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

import Foundation
import Cocoa
import OpenVanillaImp
import OVModuleManager

@objc (OVAddTableBasedInputMethodViewController)
class AddTableBasedInputMethodViewController: OVBasePreferencesViewController {

    @IBOutlet weak var moreInfoTextField: NSTextField!
    weak var preferencesWindowController: PreferencesWindowController?
    private var tablePathToBeInstalled: String?
    private var moduleIdentifierIfInstalled: String?

    override func loadPreferences() {
        super.loadPreferences()

        // add link to the more info text field
        // cf. http://developer.apple.com/library/mac/#qa/qa1487/_index.html
        let attrString = self.moreInfoTextField.attributedStringValue.mutableCopy() as! NSMutableAttributedString
        let linkRange = attrString.string.range(of: OVMainSiteURLString)
        guard let linkRange = linkRange,
              let url = URL(string: OVMainSiteURLString)else {
            return
        }
        let nsRange = NSRange(linkRange, in: attrString.string)
        attrString.beginEditing()
        attrString.addAttribute(.link, value: url, range: nsRange)
        attrString.addAttribute(.foregroundColor, value: NSColor.blue, range: nsRange)
        attrString.addAttribute(.underlineColor, value: NSUnderlineStyle.single, range: nsRange)
        attrString.endEditing()

        self.moreInfoTextField.attributedStringValue = attrString
        self.moreInfoTextField.allowsEditingTextAttributes = true
        self.moreInfoTextField.isSelectable = true
    }


    private func install(path: String) -> Bool {
        let cinPath = path

        let manager = OVModuleManager.default()
        let canInstall = manager.canInstallCustomTableBasedInputMethod(withTablePath: cinPath)

        if canInstall is OVCanNotInstall {
            NonModalAlertWindowController.shared.show(title: NSLocalizedString("Cannot Import Input Method", comment: ""), content: String(format: NSLocalizedString("\"%@\" is not a valid cin file.", comment: ""), (cinPath as NSString).lastPathComponent), confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""), cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
            return false
        }

        if let canInstall = canInstall as? OVCanInstall {
            manager.installCustomTableBasedInputMethod(withTablePath: cinPath)

            NonModalAlertWindowController.shared.show(title: NSLocalizedString("Input Method Imported", comment: ""), content: String(format: NSLocalizedString("Your new input method \"%@\" is ready to use", comment: ""), canInstall.localizedNameIfInstalled), confirmButtonTitle: NSLocalizedString("OK", comment: ""), cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
            return true
        }

        return false
    }

    @IBAction @objc(importNewTableAction:)
    func importNewTableAction(_ sender: Any) {
        let panel = NSOpenPanel()
        panel.title = NSLocalizedString("Pick the .cin Table to Import", comment: "")
        panel.level = NSWindow.Level(rawValue: Int(CGShieldingWindowLevel()) + 1)
        panel.allowsMultipleSelection = false
        panel.allowedFileTypes = ["cin"]

        panel.begin { result in
            if result != .continue {
                return
            }
            let files = panel.urls
            guard let cinPath = files.first?.path else {
                return
            }

            let manager = OVModuleManager.default()
            let canInstall = manager.canInstallCustomTableBasedInputMethod(withTablePath: cinPath)

            if canInstall is OVCanNotInstall {
                NonModalAlertWindowController.shared.show(title: NSLocalizedString("Cannot Import Input Method", comment: ""), content: String(format: NSLocalizedString("\"%@\" is not a valid cin file.", comment: ""), (cinPath as NSString).lastPathComponent), confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""), cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
                return
            }

            if let canInstall = canInstall as? OVCanInstall {
                if canInstall.willOverrideBuiltInTable {
                    self.tablePathToBeInstalled = cinPath
                    self.moduleIdentifierIfInstalled = canInstall.identifierIfInstalled

                    NonModalAlertWindowController.shared.show(title: NSLocalizedString("Overwriting Existing Input Method", comment: ""), content: String(format: NSLocalizedString("\"%@\" will replace an existing input method that you have. Do you want to continue?", comment: ""), (cinPath as NSString).lastPathComponent), confirmButtonTitle: NSLocalizedString("Overwrite", comment: ""), cancelButtonTitle: NSLocalizedString("Cancel", comment: ""), cancelAsDefault: true, delegate: self)

                }
                _ = self.install(path: cinPath)
            }
        }


    }

}


extension AddTableBasedInputMethodViewController : NonModalAlertWindowControllerDelegate {
    func nonModalAlertWindowControllerDidConfirm(_ controller: NonModalAlertWindowController) {
        if let moduleIdentifierIfInstalled = moduleIdentifierIfInstalled {
            _ = self.install(path: moduleIdentifierIfInstalled)
        }
    }

    func nonModalAlertWindowControllerDidCancel(_ controller: NonModalAlertWindowController) {
        self.tablePathToBeInstalled = nil
        self.moduleIdentifierIfInstalled = nil
    }

}
