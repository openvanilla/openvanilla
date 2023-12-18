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
import Carbon

@objc (OVBaseModulePreferencesViewController)
class BaseModulePreferencesViewController: BasePreferencesViewController {

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
