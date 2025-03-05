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
import AppKit
import Foundation
import ModuleManager

@objc(OVInputMenuPreferencesViewController)
class InputMenuPreferencesViewController: BasePreferencesViewController, NSTableViewDataSource, NSTableViewDelegate {
    @IBOutlet var tableView: NSTableView!

    override func awakeFromNib() {
        super.awakeFromNib()
        tableView.usesAutomaticRowHeights = false
        tableView.columnAutoresizingStyle = .uniformColumnAutoresizingStyle
        tableView.allowsColumnSelection = false
        tableView.allowsMultipleSelection = false
        tableView.headerView = nil
        tableView.dataSource = self
        tableView.delegate = self
        tableView.reloadData()
    }

    func numberOfRows(in tableView: NSTableView) -> Int {
        OVModuleManager.default.inputMethodIdentifiers.count
    }

    func tableView(_ tableView: NSTableView, objectValueFor tableColumn: NSTableColumn?, row: Int) -> Any? {
        let identifier = OVModuleManager.default.inputMethodIdentifiers[row]
        let array = OVModuleManager.default.excludedIdentifiers
        return array.contains(where: { $0 == identifier }) == false
    }

    func tableView(_ tableView: NSTableView, setObjectValue object: Any?, for tableColumn: NSTableColumn?, row: Int) {
        if let newValue = object as? Bool {
            let identifier = OVModuleManager.default.inputMethodIdentifiers[row]
            var array = OVModuleManager.default.excludedIdentifiers
            if newValue {
                array.removeAll { $0 == identifier }
            } else {
                if array.count >= OVModuleManager.default.inputMethodIdentifiers.count - 1 {
                    NSSound.beep()
                    return
                }
                array.append(identifier)
            }
            OVModuleManager.default.excludedIdentifiers = array
            tableView.reloadData()
        }
    }

    func tableView(_ tableView: NSTableView, shouldEdit tableColumn: NSTableColumn?, row: Int) -> Bool {
        return true
    }

    func tableView(_ tableView: NSTableView, dataCellFor tableColumn: NSTableColumn?, row: Int) -> NSCell? {
        let checkboxCell = NSButtonCell()
        checkboxCell.setButtonType(.switch)
        checkboxCell.title = ""
        checkboxCell.isEditable = true
        return checkboxCell
    }

    func tableView(_ tableView: NSTableView, willDisplayCell cell: Any, for tableColumn: NSTableColumn?, row: Int) {
        if let checkboxCell = cell as? NSButtonCell {
            let identifier = OVModuleManager.default.inputMethodIdentifiers[row]
            let title = OVModuleManager.default.localizedInputMethodName(identifier)
            checkboxCell.title = title
        }
    }
}
