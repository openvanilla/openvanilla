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

private let kGeneralSettingIdentifier = "GeneralSettingIdentifier"
private let kAssociatedPhrasesSettingIdentifier = "org.openvanilla.OVAFAssociatedPhrases"
private let kAddInputMethodIdentifier = "AddInputMethodIdentifier"

// Note: A workaround to suppresses no-selector warnings.
private final class Dummy: NSObject {
    @objc func title() -> String { "" }
    @objc func setTitle(_ title:String) {}
}

typealias PreferencesItem = (identifier: String,
                             localizedName: String,
                             viewController: BasePreferencesViewController)

@objc (OVPreferencesWindowController)
class PreferencesWindowController: NSWindowController {
    @IBOutlet weak var tableView: NSTableView!
    @IBOutlet weak var modulePreferencesContainerView: NSView!
    @IBOutlet weak var generalPreferencesViewController: BasePreferencesViewController!
    @IBOutlet weak var associatedPhrasesPreferencesViewController: BasePreferencesViewController!
    @IBOutlet weak var tableBasedModulePreferencesViewController: BasePreferencesViewController!
    @IBOutlet weak var arrayModulePreferencesViewController: BasePreferencesViewController!
    @IBOutlet weak var addTableBasedInputMethodViewController: BasePreferencesViewController!
    
    private var items: [PreferencesItem] = []
    private var localizableObjects: [NSValue: String] = [:]
    private weak var currentPreferencesViewController: BasePreferencesViewController?
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    override func windowDidLoad() {
        super.windowDidLoad()
        window?.center()
        window?.delegate = self
        reload(nil)
        
        NotificationCenter.default.addObserver(self, selector: #selector(reload(_:)), name: NSNotification.Name("OVModuleManagerDidReloadNotification"), object: nil)
        (addTableBasedInputMethodViewController as? AddTableBasedInputMethodViewController)?.preferencesWindowController = self
        
        localizableObjects.removeAll()
        
        func ListTitlesInView(_ view: NSView) {
            if view is NSPopUpButton {
                return
            }
            if let view = view as? NSTextField {
                if !view.isEditable {
                    localizableObjects[NSValue(nonretainedObject: view)] = view.stringValue
                    return
                }
            }
            if let view = view as? NSMatrix {
                for cell in view.cells {
                    localizableObjects[NSValue(nonretainedObject: cell)] = cell.title
                }
                return
            }
            let selector = #selector(Dummy.title)
            if view.responds(to: selector) {
                if let title = view.perform(selector).takeRetainedValue() as? String {
                    localizableObjects[NSValue(nonretainedObject: view)] = title
                }
            }
            
            for subview in view.subviews {
                ListTitlesInView(subview)
            }
        }
        
        ListTitlesInView(generalPreferencesViewController.view)
        ListTitlesInView(associatedPhrasesPreferencesViewController.view)
        ListTitlesInView(tableBasedModulePreferencesViewController.view)
        ListTitlesInView(arrayModulePreferencesViewController.view)
        ListTitlesInView(addTableBasedInputMethodViewController.view)
        updateLocalization()
    }
    
    // MARK: - Private methods
    
    @objc (selectInputMethodIdentifier:)
    func select(inputMethodIdentifier identifier: String) {
        
        for (index, element) in items.enumerated() {
            let elementIdentifier = element.0
            if identifier == elementIdentifier {
                tableView.selectRowIndexes(IndexSet(integer: index), byExtendingSelection: false)
                OVModuleManager.default.selectInputMethod(identifier)
                return
            }
        }
    }
    
    @objc
    func reload(_ notification: Notification?) {
        items.removeAll()
        tableView.deselectAll(self)
        items.append((kGeneralSettingIdentifier, NSLocalizedString("General Settings", comment:""), generalPreferencesViewController))
        items.append((kAssociatedPhrasesSettingIdentifier, NSLocalizedString("Associated Phrases", comment:""), associatedPhrasesPreferencesViewController))

        let identifiers = OVModuleManager.default.inputMethodIdentifiers
        for moduleIdentifier in identifiers {
            if OVModuleManager.default.isTableBasedInputMethodIdentifier(moduleIdentifier) {
                items.append((moduleIdentifier,
                              OVModuleManager.default.localizedInputMethodName(moduleIdentifier),
                              tableBasedModulePreferencesViewController))
            }
        }
        
        items.append((arrayModulePreferencesViewController.moduleIdentifier ?? "",
                      OVModuleManager.default.localizedInputMethodName(arrayModulePreferencesViewController.moduleIdentifier ?? ""),
                      arrayModulePreferencesViewController))
        
        items.append((kAddInputMethodIdentifier, NSLocalizedString("Add New Input Method", comment:""), addTableBasedInputMethodViewController))

        tableView.reloadData()
        tableView.selectRowIndexes(IndexSet(integer: 0), byExtendingSelection: false)
    }
    
    private func updateLocalization() {
        for value in localizableObjects.keys {
            guard let viewOrCell = value.nonretainedObjectValue as? NSObject,
                  let dictKey = localizableObjects[value]
            else {
                continue
            }
            
            let text = NSLocalizedString(dictKey, comment: "")
            
            if let view = viewOrCell as? NSTextField {
                if !view.isEditable {
                    view.stringValue = text
                    continue
                }
            }
            if let cell = viewOrCell as? NSCell {
                cell.title = text
                continue
            }

            let selector = #selector(Dummy.setTitle(_:))
            if viewOrCell.responds(to: selector) {
                viewOrCell.perform(selector, with: text)
            }
        }
    }
    
}

extension PreferencesWindowController: NSWindowDelegate {
    @objc
    func windowDidBecomeMain(_ notification: Notification) {
        currentPreferencesViewController?.loadPreferences()
    }
}

extension PreferencesWindowController: NSTableViewDataSource, NSTableViewDelegate {
    func tableViewSelectionDidChange(_ notification: Notification) {
        currentPreferencesViewController?.view.removeFromSuperview()
        currentPreferencesViewController = nil
        let selection = tableView.selectedRow
        if selection == -1 {
            return
        }
        let item = items[selection]
        let controller = item.viewController
        currentPreferencesViewController = controller
        if controller == tableBasedModulePreferencesViewController ||
            controller == arrayModulePreferencesViewController ||
            controller == associatedPhrasesPreferencesViewController {
            controller.moduleIdentifier = item.identifier
        }
        controller.loadPreferences()
        modulePreferencesContainerView.addSubview(controller.view)
        let title = String(format: NSLocalizedString("OpenVanilla - %@", comment: ""), item.localizedName)
        window?.title = title
    }
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        items.count
    }
    
    func tableView(_ tableView: NSTableView, objectValueFor tableColumn: NSTableColumn?, row: Int) -> Any? {
        items[row].localizedName
    }
    
}
