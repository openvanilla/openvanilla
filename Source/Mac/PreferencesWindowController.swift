import Foundation
import OVModuleManager

private let kGeneralSettingIdentifier = "GeneralSettingIdentifier"
private let kAssociatedPhrasesSettingIdentifier = "org.openvanilla.OVAFAssociatedPhrases"
private let kAddInputMethodIdentifier = "AddInputMethodIdentifier"

typealias PreferencesItem = (identifier: String,
                             localizedName: String,
                             viewController: OVBasePreferencesViewController)

@objc(OVPreferencesWindowController)
class PreferencesWindowController: NSWindowController {
    @IBOutlet weak var tableView: NSTableView!
    @IBOutlet weak var modulePreferencesContainerView: NSView!
    @IBOutlet weak var generalPreferencesViewController: OVBasePreferencesViewController!
    @IBOutlet weak var associatedPhrasesPreferencesViewController: OVBasePreferencesViewController!
    @IBOutlet weak var tableBasedModulePreferencesViewController: OVBasePreferencesViewController!
    @IBOutlet weak var arrayModulePreferencesViewController: OVBasePreferencesViewController!
    @IBOutlet weak var addTableBasedInputMethodViewController: OVBasePreferencesViewController!

    private var items: [PreferencesItem] = []
    private var localizableObjects: [NSValue:String] = [:]
    private weak var currentPreferencesViewController: OVBasePreferencesViewController?

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

        func ListTitlesInView( _ view: NSView) {
            if view is NSPopUpButton {
                return
            }
            if let view = view as? NSTextField{
                if !view.isEditable {
                    localizableObjects[NSValue(nonretainedObject: view)] = view.stringValue
                    return
                }
            }
            if let view = view as? NSMatrix {
                for cell in view.cells {
                    localizableObjects[NSValue(nonretainedObject: view)] = cell.title
                }
                return
            }
            let selector = Selector("title")
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

    @objc(selectInputMethodIdentifier:)
    func select(inputMethodIdentifier identifier: String) {

        for (index, element) in items.enumerated() {
            let elementIdentifier = element.0
            if identifier == elementIdentifier {
                tableView.selectRowIndexes(IndexSet(integer: index), byExtendingSelection: false)
                OVModuleManager.default().selectInputMethod(identifier)
                return
            }
        }
    }

    @objc
    func reload(_ notification: Notification?) {
        items.removeAll()
        tableView.deselectAll(self)
        items.append((kGeneralSettingIdentifier, "General Settings", generalPreferencesViewController))
        items.append((kAssociatedPhrasesSettingIdentifier, "Associated Phrases", associatedPhrasesPreferencesViewController))

        let identifiers = OVModuleManager.default().inputMethodIdentifiers
        for moduleIdentifier in identifiers {
            if OVModuleManager.default().isTableBasedInputMethodIdentifier(moduleIdentifier) {
                items.append((moduleIdentifier,
                              OVModuleManager.default().localizedInputMethodName(moduleIdentifier),
                              tableBasedModulePreferencesViewController))
            }
        }

        items.append((arrayModulePreferencesViewController.moduleIdentifier,
                      OVModuleManager.default().localizedInputMethodName(arrayModulePreferencesViewController.moduleIdentifier),
                      arrayModulePreferencesViewController))

        items.append((kAddInputMethodIdentifier, "Add New Input Method",  addTableBasedInputMethodViewController))

        tableView.reloadData()
        tableView.selectRowIndexes(IndexSet(integer: 0), byExtendingSelection: false)
    }

    private func updateLocalization() {
        for value in localizableObjects.keys {
            guard let view = value.nonretainedObjectValue as? NSView,
                  let dictKey = localizableObjects[value]
            else {
                continue
            }

            let text = NSLocalizedString(dictKey, comment: "")

            if let view = view as? NSTextField {
                if !view.isEditable {
                    view.stringValue = text
                    continue
                }
            }
            let selector = Selector("setTitle:")
            if view.responds(to: selector) {
                view.perform(selector, with: text)
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
