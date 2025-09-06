// Copyright (c) 2022 and onwards The McBopomofo Authors.
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

import Foundation
import SQLite

private let path = "/System/Library/PrivateFrameworks/CoreChineseEngine.framework/Versions/A/Resources/CharacterAccessibilityData.sqlite"

@objc public class CharacterInfo: NSObject {
    @objc public let character: String?
    @objc public let components: String?
    @objc public let simplifiedExample: String?
    @objc public let traditionalExample: String?

    @objc override public init() {
        character = nil
        components = nil
        simplifiedExample = nil
        traditionalExample = nil
        super.init()
    }

    @objc public init(
        character: String?,
        components: String?,
        simplifiedExample: String?,
        traditionalExample: String?
    ) {
        self.character = character
        self.components = components
        self.simplifiedExample = simplifiedExample
        self.traditionalExample = traditionalExample
        super.init()
    }
}

@objc public class SystemCharacterInfo: NSObject {
    private lazy var db: Connection? = try? Connection(path)

    @objc public func read(string: String) throws -> CharacterInfo {
        guard let db else {
            throw NSError(domain: "SystemCharacterInfo", code: -100)
        }

        let zentry = Table("ZENTRY")
        let zcharacter = SQLite.Expression<String>("ZCHARACTER")
        let query = zentry.filter(zcharacter == string).limit(1)
        do {
            let result = try db.prepare(query)
            let rows = Array(result)
            guard let row = rows.first else {
                throw NSError(domain: "SystemCharacterInfo", code: -101)
            }
            let zSimplifiedExamplar = SQLite.Expression<String?>("ZSIMPLIFIEDEXEMPLAR")
            let zTraditionalExamplar = SQLite.Expression<String?>("ZTRADITIONALEXEMPLAR")
            let zComponents = SQLite.Expression<String?>("ZCOMPONENTS")
            let zCharacter = SQLite.Expression<String?>("ZCHARACTER")

            return CharacterInfo(
                character: try? row.get(zCharacter),
                components: try? row.get(zComponents),
                simplifiedExample: try? row.get(zSimplifiedExamplar),
                traditionalExample: try? row.get(zTraditionalExamplar)
            )
        } catch {
            throw NSError(domain: "SystemCharacterInfo", code: -102)
        }
    }

    @objc public func getExplanation(string: String) -> String {
        guard db != nil else {
            return string
        }
        func getSingleExplanation(string: String) -> String? {
            do {
                let result = try read(string: string)
                if let sample = result.traditionalExample ?? result.simplifiedExample {
                    return "「\(sample)」的「\(string)」"
                }
                if let components = result.components {
                    return "「\(components)」組成的「\(string)」"
                }
            } catch {}
            return nil
        }
        var results: [String] = []
        for char in string {
            let result = getSingleExplanation(string: String(char)) ?? string
            results.append(result)
        }
        NSLog("results?? \(results)")
        switch results.count {
        case 0:
            return string
        case 1:
            return results.first ?? string
        default:
            return "\(string) \(results.joined(separator: ", "))"
        }
    }
}
