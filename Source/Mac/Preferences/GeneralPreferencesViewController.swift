import Foundation
import OpenVanillaImp

@objc (OVGeneralPreferencesViewController)
class GeneralPreferencesViewController: BasePreferencesViewController {
    @IBOutlet weak var fieldCandidateSize: NSPopUpButton!
    @IBOutlet weak var fieldCandidateStyle: NSMatrix!
    @IBOutlet weak var fieldAlphanumericKeyboardLayout: NSPopUpButton!
    @IBOutlet weak var fieldPlaySound: NSButton!
    @IBOutlet weak var fieldCheckForUpdate: NSButton!
    @IBOutlet weak var lastUpdateCheckDateLabel: NSTextField!
    @IBOutlet weak var checkForUpdateButton: NSButton!

    deinit {
        NotificationCenter.default.removeObserver(self)
    }

    override class func awakeFromNib() {
        super.awakeFromNib()
        NotificationCenter.default.addObserver(self, selector: #selector(handleUpdateCheckDidComplete(_:)), name: .UpdateCheckerDidFinishChecking, object: nil)
    }

    override func loadPreferences() {
        let userDefaults = UserDefaults.standard
        userDefaults.synchronize()

        let candidateSize = String(format: "%ju", uintmax_t(userDefaults.integer(forKey: OVCandidateListTextSizeKey)))
        fieldCandidateSize.selectItem(withTitle: candidateSize)
        if fieldCandidateSize.selectedItem == nil {
            let defaultCandidateSize = String(format: "%ju", uintmax_t(OVDefaultCandidateListTextSize))
            fieldCandidateSize.selectItem(withTitle: defaultCandidateSize)
        }


        let style = userDefaults.object(forKey: OVCandidateListStyleNameKey) as? Bool ?? false
        fieldCandidateStyle.selectCell(withTag: style ? 1 : 0)
        configureKeyboardLayoutList(fieldAlphanumericKeyboardLayout)
        fieldPlaySound.state = userDefaults.bool(forKey: OVMakeSoundFeedbackOnInputErrorKey) ? .on : .off
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
        let lastCheckString = String(format: NSLocalizedString("Last checked: %@", comment: ""), dateFormatter.string(from: lastCheckDate))
        lastUpdateCheckDateLabel.stringValue = lastCheckString
    }

    @objc (checkForUpdateAction:)
    @IBAction func checkForUpdateAction(_ sender: Any?) {
        checkForUpdateButton.isEnabled = false
        UpdateChecker.shared.checkForUpdate()
    }

    @objc (updateField:)
    @IBAction func updateField(_ sender: Any?) {
        let userDefaults = UserDefaults.standard
        if let candidateSize = fieldCandidateSize.selectedItem?.title {
            userDefaults.set((candidateSize as NSString).integerValue, forKey: OVCandidateListTextSizeKey)
        }
        userDefaults.set(fieldCandidateStyle.selectedCell()?.tag == 0 ? OVVerticalCandidateListStyleName : OVHorizontalCandidateListStyleName, forKey: OVCandidateListStyleNameKey)
        if let layout = fieldAlphanumericKeyboardLayout.selectedItem?.representedObject as? String {
            setSharedAlphanumericKeyboardLayout(layout)
        }
        userDefaults.setValue(fieldPlaySound.state == .on, forKey: OVMakeSoundFeedbackOnInputErrorKey)
        userDefaults.setValue(fieldCheckForUpdate.state == .on, forKey: OVCheckForUpdateKey)
        userDefaults.synchronize()
    }

    @objc
    func handleUpdateCheckDidComplete(_ notification: Notification) {
        checkForUpdateButton.isEnabled = true
        loadPreferences()
    }
}
