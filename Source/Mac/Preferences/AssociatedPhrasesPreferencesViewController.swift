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

private var kModuleIdentifier = "org.openvanilla.OVAFAssociatedPhrases"

@objc(OVAFAssociatedPhrasesPreferencesViewController)
class AssociatedPhrasesPreferencesViewController: BaseModulePreferencesViewController {

    @IBOutlet weak var fieldSelectionKeys: NSPopUpButton!
    @IBOutlet weak var fieldContinuousAssociation: NSButton!
    @IBOutlet weak var fieldUseSpaceAsFirstCandidateSelectionKey: NSMatrix!
    @IBOutlet weak var fieldSendFirstCandidateWithSpaceWithOnePageList: NSButton!

    private var defaultSelectionKeys: [String] = [
        "!#$%^&*()", "!#$%^&*(", "1234567890", "123456789",
    ]
    private var defaultSelectionKeyTitles: [String] = [
        "Shift-1 ~ Shift-0", "Shift-1 ~ Shift-9", "1234567890", "123456789",
    ]

    required init?(coder: NSCoder) {
        super.init(coder: coder)
        moduleIdentifier = kModuleIdentifier
    }

    private func setState(for button: NSButton, key: String) {
        button.state = boolValue(forKey: key) == true ? .on : .off
    }

    override func loadPreferences() {
        super.loadPreferences()

        fieldSelectionKeys.removeAllItems()
        guard let selectionKeys = stringValue(forKey: "SelectionKeys") else {
            return
        }
        let selectedIndex: Int? = {
            if !selectionKeys.isEmpty {
                return defaultSelectionKeys.firstIndex(of: selectionKeys)
            }
            return nil
        }()

        guard let selectedIndex = selectedIndex else {
            var titles = defaultSelectionKeyTitles
            titles.append(selectionKeys)
            fieldSelectionKeys.addItems(withTitles: titles)
            fieldSelectionKeys.selectItem(at: titles.count - 1)
            return
        }

        fieldSelectionKeys.addItems(withTitles: defaultSelectionKeyTitles)
        fieldSelectionKeys.selectItem(at: selectedIndex)
        setState(for: self.fieldContinuousAssociation, key: "ContinuousAssociation")

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
    }

    @IBAction func updateField(_ sender: NSObject?) {
        if sender == fieldUseSpaceAsFirstCandidateSelectionKey {
            if fieldUseSpaceAsFirstCandidateSelectionKey.selectedCell()?.tag ?? 0 != 0 {
                fieldSendFirstCandidateWithSpaceWithOnePageList.state = .off
            }
        } else if sender == fieldSendFirstCandidateWithSpaceWithOnePageList {
            if fieldSendFirstCandidateWithSpaceWithOnePageList.state == .on {
                fieldUseSpaceAsFirstCandidateSelectionKey.selectCell(withTag: 0)
            }
        }

        setBoolValue(self.fieldContinuousAssociation.state == .on, forKey: "ContinuousAssociation")
        var selectedIndex = self.fieldSelectionKeys.indexOfSelectedItem
        if selectedIndex == -1 {
            selectedIndex = 0
        }
        let newSelectionKeys =
            (selectedIndex < defaultSelectionKeyTitles.count
            ? defaultSelectionKeys : defaultSelectionKeyTitles)[selectedIndex]
        setStringValue(newSelectionKeys, forKey: "SelectionKeys")
        setUnsignedIntegerValue(
            UInt(fieldUseSpaceAsFirstCandidateSelectionKey.selectedCell()?.tag ?? 0),
            forKey: "UseSpaceAsFirstCandidateSelectionKey")
        setBoolValue(
            fieldSendFirstCandidateWithSpaceWithOnePageList.state == .on,
            forKey: "SendFirstCandidateWithSpaceWithOnePageList")
    }
}
