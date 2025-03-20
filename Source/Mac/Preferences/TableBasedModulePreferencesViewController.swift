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

@objc(OVTableBasedModulePreferencesViewController)
class TableBasedModulePreferencesViewController: BaseModulePreferencesViewController {

    @IBOutlet weak var fieldAlphaNumericKeyboardLayout: NSPopUpButton!
    @IBOutlet weak var fieldMaximumRadicalLength: NSPopUpButton!
    @IBOutlet weak var fieldClearReadingBufferAtCompositionError: NSButton!
    @IBOutlet weak var fieldComposeWhileTyping: NSButton!
    @IBOutlet weak var fieldSendFirstCandidateWithSpaceWithOnePageList: NSButton!
    @IBOutlet weak var fieldShouldComposeAtMaximumRadicalLength: NSButton!
    @IBOutlet weak var fieldUseSpaceAsFirstCandidateSelectionKey: NSMatrix!
    @IBOutlet weak var fieldSpecialCodePrompt: NSButton!
    @IBOutlet weak var customTableBasedInputMethodInfo: NSTextField!
    @IBOutlet weak var removeInputMethodButton: NSButton!

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
            customTableBasedInputMethodInfo.stringValue = NSLocalizedString(
                "This is a customized input method.", comment: "")
            removeInputMethodButton.isHidden = false
        } else {
            customTableBasedInputMethodInfo.stringValue = NSLocalizedString(
                "This is a built-in input method.", comment: "")
            removeInputMethodButton.isHidden = true
        }

        setState(
            for: fieldClearReadingBufferAtCompositionError,
            key: "ClearReadingBufferAtCompositionError")
        setState(for: fieldComposeWhileTyping, key: "ComposeWhileTyping")
        setState(
            for: fieldShouldComposeAtMaximumRadicalLength,
            key: "ShouldComposeAtMaximumRadicalLength")
        setState(
            for: fieldSpecialCodePrompt,
            key: "SpecialCodePrompt")

        let useSpaceAsFirstCandidateSelectionKey =
            unsignedIntegerValue(forKey: "UseSpaceAsFirstCandidateSelectionKey") ?? 0

        if useSpaceAsFirstCandidateSelectionKey != 0 {
            fieldUseSpaceAsFirstCandidateSelectionKey.selectCell(withTag: Int(
                useSpaceAsFirstCandidateSelectionKey))
            fieldSendFirstCandidateWithSpaceWithOnePageList.state = .off
        } else if boolValue(forKey: "SendFirstCandidateWithSpaceWithOnePageList") {
            fieldUseSpaceAsFirstCandidateSelectionKey.selectCell(withTag: 0)
            fieldSendFirstCandidateWithSpaceWithOnePageList.state = .on
        } else {
            fieldUseSpaceAsFirstCandidateSelectionKey.selectCell(withTag: 0)
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

    @IBAction func updateField(_ sender: NSObject) {
        if sender == fieldUseSpaceAsFirstCandidateSelectionKey {
            if fieldUseSpaceAsFirstCandidateSelectionKey.selectedCell()?.tag ?? 0 != 0 {
                fieldSendFirstCandidateWithSpaceWithOnePageList.state = .off
            }
        } else if sender == fieldSendFirstCandidateWithSpaceWithOnePageList {
            if fieldSendFirstCandidateWithSpaceWithOnePageList.state == .on {
                fieldUseSpaceAsFirstCandidateSelectionKey.selectCell(withTag: 0)
            }
        } else if sender == fieldAlphaNumericKeyboardLayout {
            if let layout = fieldAlphaNumericKeyboardLayout.selectedItem?.representedObject
                as? String
            {
                self.setStringValue(layout, forKey: OVAlphanumericKeyboardLayoutKey)
            }
        }

        setBoolValue(
            fieldClearReadingBufferAtCompositionError.state == .on,
            forKey: "ClearReadingBufferAtCompositionError")
        setBoolValue(fieldComposeWhileTyping.state == .on, forKey: "ComposeWhileTyping")
        setBoolValue(
            fieldShouldComposeAtMaximumRadicalLength.state == .on,
            forKey: "ShouldComposeAtMaximumRadicalLength")
        setUnsignedIntegerValue(
            UInt(fieldUseSpaceAsFirstCandidateSelectionKey.selectedCell()?.tag ?? 0),
            forKey: "UseSpaceAsFirstCandidateSelectionKey")
        setBoolValue(
            fieldSendFirstCandidateWithSpaceWithOnePageList.state == .on,
            forKey: "SendFirstCandidateWithSpaceWithOnePageList")
        setBoolValue(
            fieldSpecialCodePrompt.state == .on,
            forKey: "SpecialCodePrompt")

        let selectedItem = fieldMaximumRadicalLength.selectedItem
        if let selectedItem {
            let length = (selectedItem.title as NSString).integerValue
            setUnsignedIntegerValue(UInt(length), forKey: "MaximumRadicalLength")
        }
    }

    @IBAction func removeInputMethodAction(_ sender: Any?) {
        NonModalAlertWindowController.shared.show(
            title: NSLocalizedString("Confirm Removal", comment: ""),
            content: String(
                format: NSLocalizedString(
                    "Are you sure you want to remove the input method\"%@\"?", comment: ""),
                OVModuleManager.default.localizedInputMethodName(self.moduleIdentifier)),
            confirmButtonTitle: NSLocalizedString("Remove", comment: ""),
            cancelButtonTitle: NSLocalizedString("Cancel", comment: ""), cancelAsDefault: true,
            delegate: self)
    }

}

extension TableBasedModulePreferencesViewController: NonModalAlertWindowControllerDelegate {
    func nonModalAlertWindowControllerDidConfirm(_ controller: NonModalAlertWindowController) {
        do {
            try OVModuleManager.default.removeCustomTableBasedInputMethod(self.moduleIdentifier)
        } catch {
            NonModalAlertWindowController.shared.show(
                title: NSLocalizedString("Removal Failed", comment: ""),
                content: String(
                    format: NSLocalizedString(
                        "Failed to remove the input method\"%@\".\n\nError: %@", comment: ""),
                    OVModuleManager.default.localizedInputMethodName(self.moduleIdentifier),
                    error.localizedDescription),
                confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""),
                cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
        }
    }

    func nonModalAlertWindowControllerDidCancel(_ controller: NonModalAlertWindowController) {
    }

}
