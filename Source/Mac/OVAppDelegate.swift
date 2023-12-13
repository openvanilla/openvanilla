import Foundation
import Cocoa
import OVModuleManager

class OVAppDelegate: NSObject, NSApplicationDelegate {

    @IBOutlet var window: NSWindow?
    private var preferencesWindowController: PreferencesWindowController?

    func applicationDidFinishLaunching(_ notification: Notification) {
        OVModuleManager.default().reload()
        if OVAppDelegate.DebugShowPreferencesAfterAppLaunched {
            showPreferences()
        }
    }

    @objc
    func showPreferences() {
        if preferencesWindowController == nil {
            preferencesWindowController = PreferencesWindowController(windowNibName: "preferences")
        }
        preferencesWindowController?.window?.center()
        preferencesWindowController?.window?.orderFront(self)
    }

    private static var DebugShowPreferencesAfterAppLaunched = false
    @objc(setDebugShowPreferencesAfterAppLaunched:)
    static func setDebugShowPreferencesAfterAppLaunched(show: Bool) {
        DebugShowPreferencesAfterAppLaunched = show
    }

}
