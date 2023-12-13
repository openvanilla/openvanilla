import Foundation
import OpenVanillaImp

extension Notification.Name {
    static let UpdateCheckerDidFinishChecking = Notification.Name( "OVUpdateCheckerDidFinishCheckingNotification")
}

class UpdateChecker: NSObject {
    @objc(sharedInstance)
    static let shared = UpdateChecker()

    private override init() {}

    @objc
    private (set) var busy = false

    @objc
    @UserDefault(key: OVLastUpdateCheckTimeKey, defaultValue: nil)
    private (set) var lastUpdateCheckDate: Date?

    @objc
    @UserDefault(key: OVNextUpdateCheckTimeKey, defaultValue: nil)
    private (set) var nextUpdateCheckDate: Date?
    
    private var connection: URLSessionDataTask?

    func cancel() {
        connection?.cancel()
        connection = nil
    }

    @objc
    func checkForUpdate() {
        checkForUpdateForced(forced: true)
    }

    @objc
    func checkForUpdateIfNeeded() {
        checkForUpdateForced(forced: false)
    }

    private func checkForUpdateForced(forced: Bool) {

        func showPlistError() {
            if !forced {
                return
            }
            DispatchQueue.main.async {
                NonModalAlertWindowController.shared.show(title: NSLocalizedString("Update Check Failed", comment: ""), content: NSLocalizedString("The version information returned by the server is not valid.", comment: ""), confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""), cancelButtonTitle: "", cancelAsDefault: false, delegate: nil)
            }
        }

        if busy {
            return
        }
        var nextCheckDate = self.nextUpdateCheckDate
        var now = Date()
        if nextCheckDate != nil && nextCheckDate?.compare(now) != ComparisonResult.orderedAscending  {
            return
        }
        guard let url = URL(string: OVUpdateCheckInfoURLString) else {
            return
        }
        
        connection = URLSession.shared.dataTask(with: url) { data, response, error in
            self.connection = nil
            let now = Date()
            self.lastUpdateCheckDate = now
            self.nextUpdateCheckDate = Date(timeInterval: OVNextUpdateCheckRetryInterval, since: now)

            if let error {
                DispatchQueue.main.async {
                    NotificationCenter.default.post(name: .UpdateCheckerDidFinishChecking, object: self)
                }
                if forced {
                    DispatchQueue.main.async {
                        NonModalAlertWindowController.shared.show(title: NSLocalizedString("Update Check Failed", comment: ""), content: String(format: NSLocalizedString("There may be no internet connection or the server failed to respond.\n\nError message: %@", comment: ""), error.localizedDescription), confirmButtonTitle: NSLocalizedString("Dismiss", comment: ""), cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
                    }
                }
                return
            }


            let data = data ?? Data()
            guard let plist = try? PropertyListSerialization.propertyList(from: data, options: 0, format: nil) as? [AnyHashable:Any] else {
                showPlistError()
                return
            }
            DispatchQueue.main.async {
                NotificationCenter.default.post(name: .UpdateCheckerDidFinishChecking, object: self)
            }

            guard let remoteVersion = plist[kCFBundleVersionKey] as? String,
                  let remoteShortVersion = plist["CFBundleShortVersionString"] as? String
                else {
                showPlistError()
                return
            }
            guard let infoDict = Bundle.main.infoDictionary,
                  let currentVersion = infoDict[kCFBundleVersionKey as String] as? String,
                  let currentShortVersion = infoDict["CFBundleShortVersionString"] as? String else {
                return
            }

            let result = (currentVersion as NSString).compare(remoteVersion, options:.numeric)
            if result != .orderedAscending {
                if forced {
                    DispatchQueue.main.async {
                        NonModalAlertWindowController.shared.show(title: NSLocalizedString("Check for Update Completed", comment: ""), content: NSLocalizedString("You are already using the latest version of OpenVanilla.", comment: ""), confirmButtonTitle: NSLocalizedString("OK", comment: ""), cancelButtonTitle: nil, cancelAsDefault: false, delegate: nil)
                    }
                }
            }
            let versionDescriptions = plist["Description"]
            if let versionDescriptions = versionDescriptions as? [AnyHashable:Any] {
                let locale = {
                    let supportedLocales = ["en", "zh-Hant", "zh-Hans"]
                    let preferredTags = Bundle.preferredLocalizations(from: supportedLocales)
                    return preferredTags.first ?? "en"
                }()
                var versionDescription = versionDescriptions[locale] as? String  ?? ""
                if !versionDescription.isEmpty {
                    versionDescription = "\n\n" + versionDescription
                }
                let content = String(format: NSLocalizedString("You're currently using OpenVanilla %@ (%@), a new version %@ (%@) is now available. Do you want to visit openvanilla.org to download the version?%@", comment: ""), currentShortVersion, currentVersion, remoteShortVersion, remoteVersion, versionDescription)
                DispatchQueue.main.async {
                    NonModalAlertWindowController.shared.show(title: NSLocalizedString("New Version Available", comment: ""), content: content, confirmButtonTitle: NSLocalizedString("Visit Website", comment: ""), cancelButtonTitle: NSLocalizedString("Not Now", comment: ""), cancelAsDefault: false, delegate: self)
                }
            }

        }
        connection?.resume()

    }

}


extension UpdateChecker: NonModalAlertWindowControllerDelegate {
    func nonModalAlertWindowControllerDidConfirm(_ controller: NonModalAlertWindowController) {
        guard let url = URL(string: OVUpdateDownloadURLString) else { return }
        NSWorkspace.shared.open(url)
    }

    func nonModalAlertWindowControllerDidCancel(_ controller: NonModalAlertWindowController) {
        let now = Date()
        self.nextUpdateCheckDate = Date(timeInterval: OVNextUpdateCheckRemindLaterInterval, since: now)
    }
}
