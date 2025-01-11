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
import OpenVanillaImpl

@objc(OVGeneralPreferencesViewController)
class GeneralPreferencesViewController: BasePreferencesViewController {
    @IBOutlet weak var fieldCandidateSize: NSPopUpButton!
    @IBOutlet weak var fieldCandidateStyle: NSMatrix!
    @IBOutlet weak var fieldAlphanumericKeyboardLayout: NSPopUpButton!
    @IBOutlet weak var fieldAlwaysFallbackOnShift: NSButton!
    @IBOutlet weak var fieldPlaySound: NSButton!
    @IBOutlet weak var fieldCheckForUpdate: NSButton!
    @IBOutlet weak var lastUpdateCheckDateLabel: NSTextField!
    @IBOutlet weak var checkForUpdateButton: NSButton!

    deinit {
        NotificationCenter.default.removeObserver(self)
    }

    override func awakeFromNib() {
        super.awakeFromNib()
        NotificationCenter.default.addObserver(
            self, selector: #selector(handleUpdateCheckDidComplete(_:)),
            name: .UpdateCheckerDidFinishChecking, object: nil)
    }

    override func loadPreferences() {
        let userDefaults = UserDefaults.standard
        userDefaults.synchronize()

        let candidateSize = String(
            format: "%ju", uintmax_t(userDefaults.integer(forKey: OVCandidateListTextSizeKey)))
        fieldCandidateSize.selectItem(withTitle: candidateSize)
        if fieldCandidateSize.selectedItem == nil {
            let defaultCandidateSize = String(
                format: "%ju", uintmax_t(OVDefaultCandidateListTextSize))
            fieldCandidateSize.selectItem(withTitle: defaultCandidateSize)
        }

        let style = userDefaults.object(forKey: OVCandidateListStyleNameKey) as? String
        fieldCandidateStyle.selectCell(withTag: style == OVHorizontalCandidateListStyleName ? 1 : 0)
        configureKeyboardLayoutList(fieldAlphanumericKeyboardLayout)
        fieldAlwaysFallbackOnShift.state =
            userDefaults.bool(forKey: OVFallbackToAlphanumericKeyboardLayoutOnShiftKey)
            ? .on : .off
        fieldPlaySound.state =
            userDefaults.bool(forKey: OVMakeSoundFeedbackOnInputErrorKey) ? .on : .off
        fieldCheckForUpdate.state = userDefaults.bool(forKey: OVCheckForUpdateKey) ? .on : .off

        if UpdateChecker.shared.busy {
            checkForUpdateButton.isEnabled = false
        }

        let lastCheckDate = UpdateChecker.shared.lastUpdateCheckDate
        guard let lastCheckDate = lastCheckDate else {
            lastUpdateCheckDateLabel.isHidden = true
            return
        }
        lastUpdateCheckDateLabel.isHidden = false

        let dateFormatter = DateFormatter()
        dateFormatter.dateStyle = .short
        dateFormatter.timeStyle = .short
        let lastCheckString = String(
            format: NSLocalizedString("Last checked: %@", comment: ""),
            dateFormatter.string(from: lastCheckDate))
        lastUpdateCheckDateLabel.stringValue = lastCheckString
    }

    @objc(checkForUpdateAction:)
    @IBAction func checkForUpdateAction(_ sender: Any?) {
        checkForUpdateButton.isEnabled = false
        UpdateChecker.shared.checkForUpdate()
    }

    @objc(updateField:)
    @IBAction func updateField(_ sender: Any?) {
        let userDefaults = UserDefaults.standard
        if let candidateSize = fieldCandidateSize.selectedItem?.title {
            userDefaults.set(
                (candidateSize as NSString).integerValue, forKey: OVCandidateListTextSizeKey)
        }
        userDefaults.set(
            fieldCandidateStyle.selectedCell()?.tag == 0
                ? OVVerticalCandidateListStyleName : OVHorizontalCandidateListStyleName,
            forKey: OVCandidateListStyleNameKey)
        if let layout = fieldAlphanumericKeyboardLayout.selectedItem?.representedObject as? String {
            setSharedAlphanumericKeyboardLayout(layout)
        }
        userDefaults.setValue(
            fieldAlwaysFallbackOnShift.state == .on,
            forKey: OVFallbackToAlphanumericKeyboardLayoutOnShiftKey)
        userDefaults.setValue(
            fieldPlaySound.state == .on, forKey: OVMakeSoundFeedbackOnInputErrorKey)
        userDefaults.setValue(fieldCheckForUpdate.state == .on, forKey: OVCheckForUpdateKey)
        userDefaults.synchronize()
    }

    @objc
    func handleUpdateCheckDidComplete(_ notification: Notification) {
        checkForUpdateButton.isEnabled = true
        loadPreferences()
    }
}
