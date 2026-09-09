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

import Cocoa
import Foundation
import ModuleManager
import OpenVanillaImpl

private let kArrayModuleIdentifier = "org.openvanilla.OVIMArray"

@objc(OVIMArrayPreferencesViewController)
class ArrayPreferencesViewController: BaseModulePreferencesViewController {
    @IBOutlet weak var fieldAutoSP: NSButton!
    @IBOutlet weak var fieldForceSP: NSButton!
    @IBOutlet var fieldAlphaNumericKeyboardLayout: NSPopUpButton!
    @IBOutlet weak var fieldCustomArray30TableInfo: NSTextField!
    @IBOutlet weak var importArray30TableButton: NSButton!
    @IBOutlet weak var removeArray30TableButton: NSButton!

    required init?(coder: NSCoder) {
        super.init(coder: coder)
        self.moduleIdentifier = kArrayModuleIdentifier
    }

    override func loadPreferences() {
        super.loadPreferences()
        setState(for: fieldAutoSP, key: "SpecialCodePrompt")
        setState(for: fieldForceSP, key: "QuickMode")
        configureKeyboardLayoutList(fieldAlphaNumericKeyboardLayout)
        refreshCustomTableStatus()
    }

    private func setState(for button: NSButton, key: String) {
        button.state = self.boolValue(forKey: key) == true ? .on : .off
    }

    private func refreshCustomTableStatus() {
        if OVModuleManager.default.hasCustomArray30MainTable() {
            fieldCustomArray30TableInfo.stringValue = NSLocalizedString(
                "Using an imported Array30 table.", comment: "")
            removeArray30TableButton.isHidden = false
        }
        else {
            fieldCustomArray30TableInfo.stringValue = NSLocalizedString(
                "Using the built-in Array30 table.", comment: "")
            removeArray30TableButton.isHidden = true
        }
    }

    @IBAction func updateField(_ sender: NSObject?) {
        setBoolValue(fieldAutoSP.state == .on, forKey: "SpecialCodePrompt")
        setBoolValue(fieldForceSP.state == .on, forKey: "QuickMode")
        if sender == fieldAlphaNumericKeyboardLayout {
            if let layout = fieldAlphaNumericKeyboardLayout.selectedItem?.representedObject
                as? String
            {
                self.setStringValue(layout, forKey: OVAlphanumericKeyboardLayoutKey)
            }
        }
    }

    @IBAction func importArray30TableAction(_ sender: Any?) {
        guard let window = self.view.window else {
            return
        }

        let panel = NSOpenPanel()
        panel.title = NSLocalizedString("Pick the Array30 Table to Import", comment: "")
        panel.allowsMultipleSelection = false
        panel.allowedFileTypes = ["cin"]

        panel.beginSheetModal(for: window) { result in
            if result.rawValue != 1 {
                return
            }

            guard let cinPath = panel.urls.first?.path else {
                return
            }

            do {
                try OVModuleManager.default.installArray30MainTable(withTablePath: cinPath)
            }
            catch {
                NonModalAlertWindowController.shared.show(
                    title: NSLocalizedString("Cannot Import Array30 Table", comment: ""),
                    content: String(
                        format: NSLocalizedString(
                            "\"%@\" is not a valid Array30 cin file.", comment: ""),
                        (cinPath as NSString).lastPathComponent),
                    confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""),
                    cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
                return
            }

            self.refreshCustomTableStatus()

            NonModalAlertWindowController.shared.show(
                title: NSLocalizedString("Array30 Table Imported", comment: ""),
                content: NSLocalizedString(
                    "The Array30 input method now uses the imported table.", comment: ""),
                confirmButtonTitle: NSLocalizedString("OK", comment: ""), cancelButtonTitle: nil,
                cancelAsDefault: false, delegate: nil)
        }
    }

    @IBAction func removeArray30TableAction(_ sender: Any?) {
        NonModalAlertWindowController.shared.show(
            title: NSLocalizedString("Confirm Removal", comment: ""),
            content: NSLocalizedString(
                "The Array30 input method will go back to using its built-in table.",
                comment: ""),
            confirmButtonTitle: NSLocalizedString("Remove", comment: ""),
            cancelButtonTitle: NSLocalizedString("Cancel", comment: ""), cancelAsDefault: true,
            delegate: self)
    }

}

extension ArrayPreferencesViewController: NonModalAlertWindowControllerDelegate {
    func nonModalAlertWindowControllerDidConfirm(_ controller: NonModalAlertWindowController) {
        do {
            try OVModuleManager.default.removeArray30MainTable()
            refreshCustomTableStatus()
        }
        catch {
            NonModalAlertWindowController.shared.show(
                title: NSLocalizedString("Removal Failed", comment: ""),
                content: String(
                    format: NSLocalizedString(
                        "Failed to remove the Array30 table.\n\nError: %@", comment: ""),
                    error.localizedDescription),
                confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""),
                cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
        }
    }

    func nonModalAlertWindowControllerDidCancel(_ controller: NonModalAlertWindowController) {
    }

}
