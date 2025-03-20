//
// CinInstallerWindowController.swift
//
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
//

import Cocoa

protocol CinInstallerWindowControllerDelegate: AnyObject {
    func controllerDidBecomeCancelled(_ controller: CinInstallerWindowController)
    func controller(_ controller: CinInstallerWindowController, didDownload file: URL)
}

class CinInstallerWindowController: NSWindowController {
    @IBOutlet var outlineView: NSOutlineView!
    @IBOutlet var downloadButton: NSButton!
    @IBOutlet var downloadingProgressIndicator: NSProgressIndicator!
    @IBOutlet var loadingProgressIndicator: NSProgressIndicator!
    @IBOutlet var cancelButton: NSButton!
    @IBOutlet var reloadButton: NSButton!
    @IBOutlet var errorLabel: NSTextField!
    private var selectedTable: CinTable? {
        didSet {
            self.updateUI(by: downloader.state)
        }
    }
    private var manager =  CinManager()
    private var downloader = CinDownloader()
    weak var delegate: CinInstallerWindowControllerDelegate?

    override func awakeFromNib() {
        updateUI(by: manager.state)
        manager.delegate = self
        downloader.delegate = self
        self.downloadButton.title = NSLocalizedString("Download", comment: "")
        self.reloadButton.title = NSLocalizedString("Reload", comment: "")
        self.cancelButton.title = NSLocalizedString("Cancel", comment: "")
    }

    private func updateUI(by state: CinManager.State) {
        switch state {
        case .initial:
            loadingProgressIndicator.isHidden = true
            reloadButton.isEnabled = true
            errorLabel.isHidden = true
        case .loading:
            loadingProgressIndicator.isHidden = false
            loadingProgressIndicator.startAnimation(self)
            reloadButton.isEnabled = false
            downloadButton.isEnabled = false
            errorLabel.isHidden = true
        case .failed(_):
            loadingProgressIndicator.isHidden = true
            loadingProgressIndicator.stopAnimation(self)
            reloadButton.isEnabled = true
            errorLabel.isHidden = false
            errorLabel.objectValue = NSLocalizedString("Failed to load tables.", comment: "")
        case .loaded(_):
            loadingProgressIndicator.isHidden = true
            loadingProgressIndicator.stopAnimation(self)
            reloadButton.isEnabled = true
            errorLabel.isHidden = true
        case .loadedButLoading(_):
            loadingProgressIndicator.isHidden = false
            loadingProgressIndicator.startAnimation(self)
            reloadButton.isEnabled = false
            errorLabel.isHidden = true
        }
        outlineView.reloadData()
        outlineView.selectRowIndexes(IndexSet(), byExtendingSelection: false)
        selectedTable = nil
    }

    private func updateUI(by state: CinDownloader.State) {
        switch state {
        case .initial:
            downloadingProgressIndicator.stopAnimation(nil)
            downloadButton.isEnabled = selectedTable != nil
            downloadingProgressIndicator.isHidden = true
            errorLabel.isHidden = true
        case .downloading(_, let progress):
            print(progress)
            downloadButton.isEnabled = false
            downloadingProgressIndicator.startAnimation(nil)
            downloadingProgressIndicator.isHidden = false
            downloadingProgressIndicator.doubleValue = progress
            errorLabel.isHidden = true
        case .failed(_, _):
            downloadingProgressIndicator.stopAnimation(nil)
            downloadButton.isEnabled = selectedTable != nil
            downloadingProgressIndicator.isHidden = true
            errorLabel.isHidden = false
            errorLabel.objectValue = NSLocalizedString("Failed to download.", comment: "")
        case .downloaded(_, _):
            downloadingProgressIndicator.stopAnimation(nil)
            downloadButton.isEnabled = selectedTable != nil
            downloadingProgressIndicator.isHidden = true
            errorLabel.isHidden = true
        }
    }

    func reset() {
        manager.reset()
        downloader.reset()
    }

    @IBAction func cancel(_ sender: AnyObject?) {
        downloader.cancel()
        self.delegate?.controllerDidBecomeCancelled(self)
    }

    @IBAction func download(_ sender: AnyObject?) {
        if let selectedTable {
            downloader.download(table: selectedTable)
        }
    }

    @IBAction func reload(_ sender: AnyObject?) {
        manager.load()
    }
}

extension CinInstallerWindowController: CinManagerDelegate, CinDownloaderDelegate {
    func cinManager(_ manager: CinManager, didUpdate state: CinManager.State) {
        updateUI(by: state)
    }

    func cinDownloader(_ downloader: CinDownloader, didUpdate state: CinDownloader.State) {
        updateUI(by: state)
        if case .downloaded(_, let downloadedLocation) = state {
            self.delegate?.controller(self, didDownload: downloadedLocation)
        }
    }
}

// MARK: - NSOutlineView

extension CinInstallerWindowController: NSOutlineViewDelegate, NSOutlineViewDataSource {
    func currentCinGroup() -> [CinTableGroup]? {
        let state = manager.state
        switch state {
        case let .loaded(tableGroups):
            return tableGroups
        case let .loadedButLoading(tableGroups):
            return tableGroups
        default:
            return nil
        }
    }

    func outlineView(_ outlineView: NSOutlineView, numberOfChildrenOfItem item: Any?) -> Int {
        guard let mode = currentCinGroup() else {
            return 0
        }
        switch item {
        case nil:
            return mode.count
        case let group as CinTableGroup:
            return group.tables.count
        default:
            return 0
        }
    }

    func outlineView(_ outlineView: NSOutlineView, child index: Int, ofItem item: Any?) -> Any {
        guard let mode = currentCinGroup() else {
            fatalError("Invalid index")
        }

        switch item {
        case nil:
            return mode[index]
        case let group as CinTableGroup:
            return group.tables[index]
        default:
            fatalError("Invalid index")
        }
    }

    func outlineView(_ outlineView: NSOutlineView, isItemExpandable item: Any) -> Bool {
        return item is CinTableGroup
    }

    func outlineView(
        _ outlineView: NSOutlineView, objectValueFor tableColumn: NSTableColumn?, byItem item: Any?
    ) -> Any? {
        switch tableColumn?.identifier.rawValue {
        case "title":
            switch item {
            case let group as CinTableGroup:
                return group.name
            case let table as CinTable:
                return table.filename
            default:
                return ""
            }
        case "description":
            switch item {
            case let table as CinTable:
                return table.shortDescription
            default:
                return ""
            }
        default:
            return ""
        }
    }

    func outlineViewSelectionDidChange(_ notification: Notification) {
        let selectedRow = outlineView.selectedRow
        guard selectedRow >= 0,
            let item = outlineView.item(atRow: selectedRow)
        else {
            selectedTable = nil
            return
        }
        selectedTable = item as? CinTable
    }
}
