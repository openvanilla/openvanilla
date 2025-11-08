// Copyright (c) 2004-2012 Lukhnos Liu (lukhnos at openvanilla dot org)
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without

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
