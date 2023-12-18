import Foundation

private let kArrayModuleIdentifier = "org.openvanilla.OVIMArray"

@objc (OVIMArrayPreferencesViewController)
class ArrayPreferencesViewController: OVBaseModulePreferencesViewController {
    @IBOutlet weak var fieldAutoSP: NSButton!
    @IBOutlet weak var fieldForceSP: NSButton!
 
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        self.moduleIdentifier = kArrayModuleIdentifier
    }

    override func loadPreferences() {
        super.loadPreferences()
        setState(for: fieldAutoSP, key: "SpecialCodePrompt")
        setState(for: fieldForceSP, key: "QuickMode")
    }

    private func setState(for button: NSButton, key: String) {
        button.state = self.boolValue(forKey: key) == true ? .on : .off
    }

    @IBAction func updateField(_ sender: Any?) {
        setBoolValue(fieldAutoSP.state == .on, forKey: "SpecialCodePrompt")
        setBoolValue(fieldForceSP.state == .on, forKey: "QuickMode")
    }
}
