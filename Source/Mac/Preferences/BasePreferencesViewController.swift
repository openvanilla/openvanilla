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
import OVModuleManager

@objc (BasePreferencesViewController)
class BasePreferencesViewController: NSViewController {

    var moduleIdentifier: String?

    func configureKeyboardLayoutList(_ popUpButton: NSPopUpButton ) {
        let defaultIdentifier = OVModuleManager.default().sharedAlphanumericKeyboardLayoutIdentifier
        var layoutIdentifier = defaultIdentifier
        if let moduleIdentifier = moduleIdentifier {
            layoutIdentifier = OVModuleManager.default().alphanumericKeyboardLayout(forInputMethod: moduleIdentifier)
        }

        let list = TISCreateInputSourceList(nil, true).takeRetainedValue() as! [TISInputSource]
        var usKeyboardLayoutItem: NSMenuItem?
        var chosenItem: NSMenuItem?

        popUpButton.menu?.removeAllItems()

        for source in list {

            func getString(_ key: CFString) -> String? {
                if let ptr = TISGetInputSourceProperty(source, key) {
                    return String(Unmanaged<CFString>.fromOpaque(ptr).takeUnretainedValue())
                }
                return nil
            }

            func getBool(_ key: CFString) -> Bool? {
                if let ptr = TISGetInputSourceProperty(source, key) {
                    return Unmanaged<CFBoolean>.fromOpaque(ptr).takeUnretainedValue() == kCFBooleanTrue
                }
                return nil
            }

            if let category = getString(kTISPropertyInputSourceCategory) {
                if category != String(kTISCategoryKeyboardInputSource) {
                    continue
                }
            } else {
                continue
            }

            if let asciiCapable = getBool(kTISPropertyInputSourceIsASCIICapable) {
                if !asciiCapable {
                    continue
                }
            } else {
                continue
            }

            if let sourceType = getString(kTISPropertyInputSourceType) {
                if sourceType != String(kTISTypeKeyboardLayout) {
                    continue
                }
            } else {
                continue
            }


            guard let sourceID = getString(kTISPropertyInputSourceID),
                  let localizedName = getString(kTISPropertyLocalizedName) else {
                continue
            }

            let menuItem = NSMenuItem()
            menuItem.title = localizedName
            menuItem.representedObject = sourceID

            if let iconPtr = TISGetInputSourceProperty(source, kTISPropertyIconRef) {
                let icon = IconRef(iconPtr)
                let image = NSImage(iconRef: icon)

                func resize(_ image: NSImage) -> NSImage {
                    let newImage = NSImage(size: NSSize(width: 16, height: 16))
                    newImage.lockFocus()
                    image.draw(in: NSRect(x: 0, y: 0, width: 16, height: 16))
                    newImage.unlockFocus()
                    return newImage
                }

                let resizedImage = resize(image)
                // On newer version of macOS, the icons became black and white
                // so we make them template images so it could look better
                // on dark mode.
                if #available(macOS 10.16, *) {
                    resizedImage.isTemplate = true
                }
                menuItem.image = resizedImage
            }

            if sourceID == defaultIdentifier {
                usKeyboardLayoutItem = menuItem
            }

            if layoutIdentifier == sourceID {
                chosenItem = menuItem
            }
        }

        if let chosenItem {
            popUpButton.select(chosenItem)
        } else if let usKeyboardLayoutItem {
            popUpButton.select(usKeyboardLayoutItem)
        }
    }


    func setSharedAlphanumericKeyboardLayout(_ identifier: String ) {
        OVModuleManager.default().sharedAlphanumericKeyboardLayoutIdentifier = identifier
    }

    func loadPreferences() {}
}

