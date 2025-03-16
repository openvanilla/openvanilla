//
// CinDownloader.swift
//
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

import Cocoa

@MainActor
class CinDownloader: NSObject {
    protocol Delegate: AnyObject {
        func cinDownloader(_ downloader: CinDownloader, didUpdate state: State)
    }

    enum State {
        case initial
        case downloading(table: CinTable, progress: Double)
        case failed(table: CinTable, error: Error)
        case downloaded(table: CinTable, downloadedLocation: URL)
    }

    weak var delegate: Delegate?

    var state = State.initial {
        didSet {
            self.delegate?.cinDownloader(self, didUpdate: state)
        }
    }

    enum CinDownloaderError: Error, LocalizedError {
        case noFile
        case cancelled
        var noFile: String? {
            switch self {
            case .noFile:
                return "Downloaaded file does not exist."
            case .cancelled:
                return "Cancelled"
            }
        }
    }

    var task: URLSessionDownloadTask?
    lazy var session: URLSession = {
        let session = URLSession(configuration: .default, delegate: self, delegateQueue: nil)
        return session
    }()

    func download(table: CinTable) {
        if case .downloading = state {
            return
        }
        let request = URLRequest(url: table.url)
        self.state = .downloading(table: table, progress: 0)

        let task = session.downloadTask(with: request) { location, response, error in
            if let error {
                DispatchQueue.main.async {
                    self.task = nil
                    self.state = .failed(table: table, error: error)
                }
                return
            }
            guard let location else {
                DispatchQueue.main.async {
                    self.task = nil
                    self.state = .failed(table: table, error: CinDownloaderError.noFile)
                }
                return
            }
            DispatchQueue.main.async {
                self.task = nil
                self.state = .downloading(table: table, progress: 1.0)
                self.state = .downloaded(table: table, downloadedLocation: location)
            }
        }
        self.task = task
        task.resume()
    }

    func cancel() {
        self.task?.cancel()
        self.task = nil
    }

    func reset() {
        self.task?.cancel()
        self.task = nil
        self.state = .initial
    }
}

extension CinDownloader: URLSessionDelegate {
    func urlSession(
        _ session: URLSession, downloadTask: URLSessionDownloadTask,
        didWriteData bytesWritten: Int64,
        totalBytesWritten: Int64,
        totalBytesExpectedToWrite: Int64
    ) {
        if case let .downloading(table, _) = state {
            let progress = Double(totalBytesWritten) / Double(totalBytesExpectedToWrite)
            DispatchQueue.main.async {
                self.state = .downloading(table: table, progress: progress)
            }
        }
    }
}
