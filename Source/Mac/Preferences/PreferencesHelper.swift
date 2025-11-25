// Copyright (c) 2025 and onwards The OpenVanilla Authors.
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

import Carbon

class PreferencesHelper {

    static func boolValue(moduleIdentifier: String, forKey key: String) -> Bool {
        let value =
            CFPreferencesCopyValue(
                key as CFString, moduleIdentifier as CFString, kCFPreferencesCurrentUser,
                kCFPreferencesAnyHost) as? Bool
        return value ?? false
    }

    static func setBoolValue(moduleIdentifier: String, value: Bool, forKey key: String) {
        CFPreferencesSetValue(
            key as CFString, value ? kCFBooleanTrue : kCFBooleanFalse, moduleIdentifier as CFString,
            kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
        CFPreferencesSynchronize(
            moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
    }

    static func stringValue(moduleIdentifier: String, forKey key: String) -> String? {
        let value = CFPreferencesCopyValue(
            key as CFString, moduleIdentifier as CFString, kCFPreferencesCurrentUser,
            kCFPreferencesAnyHost)
        return value as? String
    }

    static func setStringValue(moduleIdentifier: String, value: String, forKey key: String) {
        CFPreferencesSetValue(
            key as CFString, value as CFString, moduleIdentifier as CFString,
            kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
        CFPreferencesSynchronize(
            moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
    }

    static func unsignedIntegerValue(moduleIdentifier: String, forKey key: String) -> UInt? {
        let value =
            CFPreferencesCopyValue(
                key as CFString, moduleIdentifier as CFString, kCFPreferencesCurrentUser,
                kCFPreferencesAnyHost) as? NSNumber
        return value?.uintValue
    }

    static func setUnsignedIntegerValue(moduleIdentifier: String, value: UInt, forKey key: String) {
        CFPreferencesSetValue(
            key as CFString, NSNumber(value: value) as CFNumber, moduleIdentifier as CFString,
            kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
        CFPreferencesSynchronize(
            moduleIdentifier as CFString, kCFPreferencesCurrentUser, kCFPreferencesAnyHost)
    }

}
