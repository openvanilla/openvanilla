//
// CinManager.swift
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

import Foundation

private let CinRepositoryBaseURL =
    "https://raw.githubusercontent.com/chinese-opendesktop/cin-tables/refs/heads/master"

struct CinTable {
    var filename: String
    var url: URL
    var name: String
    var shortDescription: String
}

struct CinTableGroup {
    var name: String
    var tables: [CinTable]

    mutating func insert(_ table: CinTable) {
        tables.append(table)
    }
}

private struct CinReadMeParser {
    static func parse(text: String) -> [CinTableGroup] {
        let lines = text.split(separator: "\n")
        let groupRegex = try! NSRegularExpression(pattern: "Language\\s=\\s\"(.*),(.*)\"")
        let tableRegex = try! NSRegularExpression(pattern: "(\\w+)\\.cin,\"(.*)\",\"(.*)\"")

        var groups: [CinTableGroup] = []
        var currentGroup: CinTableGroup?

        for line in lines {
            if let range = groupRegex.firstMatch(
                in: String(line), range: NSRange(location: 0, length: line.utf16.count))
            {
                print("range \(range)")
                let groupNameRange = range.range(at: 1)
                let groupName = String(
                    line[
                        line.index(
                            line.startIndex, offsetBy: groupNameRange.lowerBound)..<line.index(
                                line.startIndex, offsetBy: groupNameRange.upperBound)])

                if let currentGroup = currentGroup {
                    groups.append(currentGroup)
                }

                currentGroup = CinTableGroup(name: groupName, tables: [])
                continue
            }

            if let range = tableRegex.firstMatch(
                in: String(line), range: NSRange(location: 0, length: line.utf16.count))
            {
                print("range \(range)")
                let filenameRange = range.range(at: 1)
                let filename =
                    String(
                        line[
                            line.index(
                                line.startIndex, offsetBy: filenameRange.lowerBound)..<line.index(
                                    line.startIndex, offsetBy: filenameRange.upperBound)]) + ".cin"
                let url = CinRepositoryBaseURL + "/" + filename
                let nameRange = range.range(at: 2)
                let name = String(
                    line[
                        line.index(
                            line.startIndex, offsetBy: nameRange.lowerBound)..<line.index(
                                line.startIndex, offsetBy: nameRange.upperBound)])
                let shortDescriptionRange = range.range(at: 3)
                let shortDescription = String(
                    line[
                        line.index(
                            line.startIndex, offsetBy: shortDescriptionRange.lowerBound)..<line
                            .index(line.startIndex, offsetBy: shortDescriptionRange.upperBound)])
                if let url = URL(string: url) {
                    currentGroup?.insert(
                        CinTable(
                            filename: filename, url: url, name: name,
                            shortDescription: shortDescription))
                }
            }
        }
        if let currentGroup = currentGroup {
            groups.append(currentGroup)
        }
        return groups
    }
}

protocol CinManagerDelegate: AnyObject {
    func cinManager(_ manager: CinManager, didUpdate state: CinManager.State)
}


@MainActor
class CinManager {

    enum State {
        case initial
        case failed(error: Error)
        case loading
        case loaded(tableGroups: [CinTableGroup])
        case loadedButLoading(tableGroups: [CinTableGroup])
    }

    enum CinManagerError: Error, LocalizedError {
        case noData
        var errorDescription: String? {
            "No data found."
        }
    }

    weak var delegate: CinManagerDelegate?

    var state = State.initial {
        didSet {
            self.delegate?.cinManager(self, didUpdate: state)
        }
    }

    func reset() {
        self.task?.cancel()
        self.task = nil
        self.state = .initial
    }

    private var task: URLSessionDataTask?

    func load() {
        let readmeURLString = "README"
        guard let url = URL(string: CinRepositoryBaseURL + "/" + readmeURLString) else {
            return
        }

        switch state {
        case .loading, .loadedButLoading:
            return
        case .initial, .failed:
            self.state = .loading
        case let .loaded(tableGroups):
            self.state = .loadedButLoading(tableGroups: tableGroups)
        }

        task?.cancel()
        task = nil

        task = URLSession.shared.dataTask(with: url) { [weak self] data, _, error in
            guard let self else {
                return
            }
            if let error {
                DispatchQueue.main.async {
                    self.task = nil
                    if case let .loadedButLoading(tabkeGroup) = self.state {
                        self.state = .loaded(tableGroups: tabkeGroup)
                    } else {
                        self.state = .failed(error: error)
                    }
                }
                return
            }
            guard let data = data, let string = String(data: data, encoding: .utf8) else {
                DispatchQueue.main.async {
                    self.task = nil
                    if case let .loadedButLoading(tabkeGroup) = self.state {
                        self.state = .loaded(tableGroups: tabkeGroup)
                    } else {
                        self.state = .failed(error: CinManagerError.noData)
                    }
                }
                return
            }
            let result = CinReadMeParser.parse(text: string)
            DispatchQueue.main.async {
                self.task = nil
                self.state = .loaded(tableGroups: result)
            }
        }
        task?.resume()
    }

}
