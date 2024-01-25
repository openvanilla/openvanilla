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
import OpenVanillaImpl
import ModuleManager

@objc (OVTableBasedModulePreferencesViewController)
class TableBasedModulePreferencesViewController: BaseModulePreferencesViewController {

    @IBOutlet var fieldAlphaNumericKeyboardLayout: NSPopUpButton!
    @IBOutlet var fieldMaximumRadicalLength: NSPopUpButton!
    @IBOutlet var fieldClearReadingBufferAtCompositionError: NSButton!
    @IBOutlet var fieldComposeWhileTyping: NSButton!
    @IBOutlet var fieldSendFirstCandidateWithSpaceWithOnePageList: NSButton!
    @IBOutlet var fieldShouldComposeAtMaximumRadicalLength: NSButton!
    @IBOutlet var fieldUseSpaceAsFirstCandidateSelectionKey: NSButton!
    @IBOutlet var cusmtomTableBasedInputMethodInfo: NSTextField!
    @IBOutlet var removeInputMethodButton: NSButton!

    override var moduleIdentifier: String! {
        didSet {
            let controller = NonModalAlertWindowController.shared
            if controller.window?.isVisible == true {
                controller.cancel(nil)
            }
        }
    }

    private func setState(for button: NSButton, key: String) {
        button.state = self.boolValue(forKey: key) == true ? .on : .off
    }

    override func loadPreferences() {
        super.loadPreferences()

        let manager = OVModuleManager.default
        if manager.isCustomTableBasedInputMethod(moduleIdentifier) {
            cusmtomTableBasedInputMethodInfo.stringValue = NSLocalizedString("This is a customized input method.", comment: "")
            removeInputMethodButton.isHidden = false
        } else {
            cusmtomTableBasedInputMethodInfo.stringValue = NSLocalizedString("This is a built-in input method.", comment: "")
            removeInputMethodButton.isHidden = true
        }

        setState(for: fieldClearReadingBufferAtCompositionError, key: "ClearReadingBufferAtCompositionError")
        setState(for: fieldComposeWhileTyping, key: "ComposeWhileTyping")
        setState(for: fieldShouldComposeAtMaximumRadicalLength, key: "ShouldComposeAtMaximumRadicalLength")

        if boolValue(forKey: "UseSpaceAsFirstCandidateSelectionKey") {
            fieldUseSpaceAsFirstCandidateSelectionKey.state = .on
            fieldSendFirstCandidateWithSpaceWithOnePageList.state = .off
        } else if boolValue(forKey: "SendFirstCandidateWithSpaceWithOnePageList") {
            fieldUseSpaceAsFirstCandidateSelectionKey.state = .on
            fieldSendFirstCandidateWithSpaceWithOnePageList.state = .on
        } else {
            fieldUseSpaceAsFirstCandidateSelectionKey.state = .off
            fieldSendFirstCandidateWithSpaceWithOnePageList.state = .off
        }

        var length = unsignedIntegerValue(forKey: "MaximumRadicalLength") ?? 0
        if length == 0 || length > 100 {
            length = 5
        }

        fieldMaximumRadicalLength.selectItem(withTitle: "5")

        for item in fieldMaximumRadicalLength.menu?.items ?? [] {
            if (item.title as NSString).integerValue == length {
                fieldMaximumRadicalLength.select(item)
                break
            }
        }

        configureKeyboardLayoutList(fieldAlphaNumericKeyboardLayout)
    }


    @IBAction func updateField(_  sender: NSObject) {
        if sender == fieldUseSpaceAsFirstCandidateSelectionKey {
            if fieldUseSpaceAsFirstCandidateSelectionKey.state == .on {
                fieldSendFirstCandidateWithSpaceWithOnePageList.state = .off
            }
        } else if sender == fieldSendFirstCandidateWithSpaceWithOnePageList {
            if fieldSendFirstCandidateWithSpaceWithOnePageList.state == .on {
                fieldUseSpaceAsFirstCandidateSelectionKey.state = .off
            }
        } else if sender == fieldAlphaNumericKeyboardLayout {
            if let layout = fieldAlphaNumericKeyboardLayout.selectedItem?.representedObject as? String {
                self.setStringValue(layout, forKey: OVAlphanumericKeyboardLayoutKey)
            }
        }

        setBoolValue(fieldClearReadingBufferAtCompositionError.state == .on, forKey: "ClearReadingBufferAtCompositionError")
        setBoolValue(fieldComposeWhileTyping.state == .on, forKey: "ComposeWhileTyping")
        setBoolValue(fieldShouldComposeAtMaximumRadicalLength.state == .on, forKey: "ShouldComposeAtMaximumRadicalLength")
        setBoolValue(fieldUseSpaceAsFirstCandidateSelectionKey.state == .on, forKey: "UseSpaceAsFirstCandidateSelectionKey")
        setBoolValue(fieldSendFirstCandidateWithSpaceWithOnePageList.state == .on, forKey: "SendFirstCandidateWithSpaceWithOnePageList")

        let selectedItem = fieldMaximumRadicalLength.selectedItem
        if let selectedItem {
            let length = (selectedItem.title as NSString).integerValue
            setUnsignedIntegerValue(UInt(length), forKey: "MaximumRadicalLength")
        }
    }

    @IBAction func removeInputMethodAction(_ sender: Any?) {
        NonModalAlertWindowController.shared.show(title: NSLocalizedString("Confirm Removal", comment: ""), content: String(format: NSLocalizedString("Are you sure you want to remove the input method\"%@\"", comment: ""), OVModuleManager.default.localizedInputMethodName(self.moduleIdentifier)), confirmButtonTitle: NSLocalizedString("Remove", comment: ""), cancelButtonTitle: NSLocalizedString("Cancel", comment: ""), cancelAsDefault: true, delegate: self)
    }

}

extension TableBasedModulePreferencesViewController: NonModalAlertWindowControllerDelegate {
    func nonModalAlertWindowControllerDidConfirm(_ controller: NonModalAlertWindowController) {
        do {
            try OVModuleManager.default.removeCustomTableBasedInputMethod(self.moduleIdentifier)
        } catch {
            NonModalAlertWindowController.shared.show(title: NSLocalizedString("Removal Failed", comment: ""), content: String(format: NSLocalizedString("Failed to remove the input method\"%@\".\n\nError: %@", comment: ""), OVModuleManager.default.localizedInputMethodName(self.moduleIdentifier), error.localizedDescription), confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""), cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
        }
    }

    func nonModalAlertWindowControllerDidCancel(_ controller: NonModalAlertWindowController) {
    }

}
