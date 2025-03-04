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
        let array = OVModuleManager.default.inputMethodFilterArray
        return array.contains(where: { $0 == identifier }) == false
    }

    func tableView(_ tableView: NSTableView, setObjectValue object: Any?, for tableColumn: NSTableColumn?, row: Int) {
        if let newValue = object as? Bool {
            let identifier = OVModuleManager.default.inputMethodIdentifiers[row]
            var array = OVModuleManager.default.inputMethodFilterArray
            if newValue {
                array.removeAll { $0 == identifier }
            } else {
                if array.count >= OVModuleManager.default.inputMethodIdentifiers.count - 1 {
                    NSSound.beep()
                    return
                }
                array.append(identifier)
            }
            OVModuleManager.default.inputMethodFilterArray = array
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
