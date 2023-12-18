import Foundation
import Carbon

@objc (OVBaseModulePreferencesViewController)
class BaseModulePreferencesViewController: OVBasePreferencesViewController {

    func boolValue(forKey key: String) -> Bool {
        guard let moduleIdentifier = self.moduleIdentifier else {
            return false
        }
        let value = CFPreferencesCopyValue(key as CFString, moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost) as? Bool
        return value ?? false
    }

    func setBoolValue(_ value: Bool, forKey key: String) {
        guard let moduleIdentifier = self.moduleIdentifier else {
            return
        }
        CFPreferencesSetValue(key as CFString, value ? kCFBooleanTrue : kCFBooleanFalse, moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
        CFPreferencesSynchronize(moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
    }

    func stringValue(forKey key: String) -> String? {
        guard let moduleIdentifier = self.moduleIdentifier else {
            return nil
        }
        let value = CFPreferencesCopyValue(key as CFString, moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
        return value as? String
    }

    func setStringValue(_ value: String, forKey key: String) {
        guard let moduleIdentifier = self.moduleIdentifier else {
            return
        }
        CFPreferencesSetValue(key as CFString, value as CFString, moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
        CFPreferencesSynchronize(moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
    }

    func unsignedIntegerValue(forKey key: String) -> UInt? {
        guard let moduleIdentifier = self.moduleIdentifier else {
            return nil
        }
        let value = CFPreferencesCopyValue(key as CFString, moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost) as? NSNumber
        return value?.uintValue
    }

    func setUnsignedIntegerValue(_ value: UInt, forKey key: String) {
        guard let moduleIdentifier = self.moduleIdentifier else {
            return
        }
        CFPreferencesSetValue(key as CFString, NSNumber(value: value) as CFNumber, moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost);
        CFPreferencesSynchronize(moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
    }
}
