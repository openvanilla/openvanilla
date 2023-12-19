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
import Cocoa

private let kArrayModuleIdentifier = "org.openvanilla.OVIMArray"

@objc (OVIMArrayPreferencesViewController)
class ArrayPreferencesViewController: BaseModulePreferencesViewController {
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
