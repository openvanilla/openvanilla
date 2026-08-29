import Foundation
import XCTest
import OVIMArray
import OpenVanillaTestSupport

final class OVIMArrayTests: XCTestCase {
    func testIdentifierAndLocalizedNames() {
        var inputMethod = OpenVanilla.OVIMArray("")

        XCTAssertEqual(String(inputMethod.identifier()), "org.openvanilla.OVIMArray")
        XCTAssertEqual(String(inputMethod.localizedName("en")), "Array")
        XCTAssertEqual(String(inputMethod.localizedName("zh-Hant")), "行列")
    }

    func testDefaultSpecialCodeConfiguration() {
        var inputMethod = OpenVanilla.OVIMArray("")

        XCTAssertTrue(inputMethod.isAutoSP())
        XCTAssertFalse(inputMethod.isForceSP())

        inputMethod.setForceSP(true)
        XCTAssertTrue(inputMethod.isForceSP())

        inputMethod.setForceSP(false)
        XCTAssertFalse(inputMethod.isForceSP())
    }

    func testMissingTablesFailGracefullyAndAreNotRetried() {
        var inputMethod = OpenVanilla.OVIMArray("/path/that/does/not/exist")

        XCTAssertFalse(inputMethod.checkTable())
        XCTAssertNil(inputMethod.createContext())

        XCTAssertFalse(inputMethod.checkTable())
        XCTAssertNil(inputMethod.createContext())
    }

    func testLoadsTablesFromRepositoryDataTablesPath() throws {
        let tableRoot = try Self.repositoryDataTablesPath()
        var inputMethod = OpenVanilla.OVIMArray(std.string(tableRoot))

        XCTAssertTrue(inputMethod.checkTable())
        XCTAssertNotNil(inputMethod.createContext())
    }

    func testHandleKeyShowsReadingAndShortCodeCandidates() throws {
        var harness = try Self.makeContextHarness()

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.a)))

        XCTAssertEqual(String(harness.readingText()), "1-")
        XCTAssertTrue(harness.candidatePanelVisible())
        XCTAssertGreaterThan(harness.candidateCount(), 0)
        XCTAssertEqual(String(harness.candidateAt(0)), "一")
    }

    func testHandleKeyCommitsSingleMainTableCandidateWithSpace() throws {
        var harness = try Self.makeContextHarness()

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.a)))
        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.space)))

        XCTAssertEqual(String(harness.committedText()), "一")
        XCTAssertEqual(String(harness.readingText()), "")
        XCTAssertFalse(harness.candidatePanelVisible())
    }

    func testHandleKeyBackspaceClearsReadingText() throws {
        var harness = try Self.makeContextHarness()

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.a)))
        XCTAssertEqual(String(harness.readingText()), "1-")

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.backspace)))
        XCTAssertEqual(String(harness.readingText()), "")
        XCTAssertFalse(harness.candidatePanelVisible())
    }

    func testHandleKeyTogglesForceSPWithSpecialSequence() throws {
        var harness = try Self.makeContextHarness()

        for keyCode in [CharacterCode.comma, CharacterCode.comma, CharacterCode.s, CharacterCode.p] {
            XCTAssertTrue(harness.handleKey(Int32(keyCode)))
        }

        XCTAssertFalse(harness.isForceSP())
        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.space)))

        XCTAssertTrue(harness.isForceSP())
        XCTAssertEqual(String(harness.readingText()), "")
    }

    func testHandleKeyApostropheCommitsCommaCommaAsPhraseLookupFailure() throws {
        var harness = try Self.makeContextHarness()

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.comma)))
        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.comma)))
        XCTAssertEqual(String(harness.readingText()), "8⇣8⇣")
        XCTAssertTrue(harness.candidatePanelVisible())

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.apostrophe)))

        XCTAssertEqual(String(harness.readingText()), "")
        XCTAssertEqual(String(harness.committedText()), "")
        XCTAssertFalse(harness.candidatePanelVisible())
        XCTAssertEqual(String(harness.toolTipText()), "無此詞彙")
    }

    func testHandleKeyApostropheCommitsCommaCommaCommaPhrase() throws {
        var harness = try Self.makeContextHarness()

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.comma)))
        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.comma)))
        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.comma)))
        XCTAssertEqual(String(harness.readingText()), "8⇣8⇣8⇣")

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.apostrophe)))

        XCTAssertEqual(String(harness.committedText()), "米糕")
        XCTAssertEqual(String(harness.readingText()), "")
        XCTAssertFalse(harness.candidatePanelVisible())
    }

    func testHandleKeyUACWildcardSpaceShowsMainTableCandidates() throws {
        var harness = try Self.makeContextHarness()

        for keyCode in [CharacterCode.u, CharacterCode.a, CharacterCode.c, CharacterCode.asterisk] {
            XCTAssertTrue(harness.handleKey(Int32(keyCode)))
        }
        XCTAssertEqual(String(harness.readingText()), "7⇡1-3⇣＊")

        XCTAssertTrue(harness.handleKey(Int32(CharacterCode.space)))

        XCTAssertEqual(String(harness.readingText()), "7⇡1-3⇣＊")
        XCTAssertEqual(String(harness.committedText()), "")
        XCTAssertEqual(String(harness.composingText()), "縣")
        XCTAssertTrue(harness.candidatePanelVisible())
        XCTAssertGreaterThan(harness.candidateCount(), 1)
        XCTAssertEqual(String(harness.candidateAt(0)), "縣")
    }

    private static func repositoryDataTablesPath() throws -> String {
        let testFileURL = URL(fileURLWithPath: #filePath)
        let packageRoot = testFileURL
            .deletingLastPathComponent()
            .deletingLastPathComponent()
            .deletingLastPathComponent()
        let dataTablesURL = packageRoot
            .deletingLastPathComponent()
            .deletingLastPathComponent()
            .appendingPathComponent("DataTables")

        for filename in requiredArrayTableFilenames {
            let path = dataTablesURL.appendingPathComponent(filename).path
            XCTAssertTrue(FileManager.default.fileExists(atPath: path), "Missing required table: \(path)")
        }

        return dataTablesURL.path
    }

    private static let requiredArrayTableFilenames = [
        "array30.cin",
        "array-shortcode.cin",
        "array-special.cin",
        "array-phrase.cin",
    ]

    private static func makeContextHarness() throws -> OpenVanillaTestSupport.OVIMArrayContextTestHarness {
        let harness = OpenVanillaTestSupport.OVIMArrayContextTestHarness(std.string(try repositoryDataTablesPath()))
        XCTAssertTrue(harness.isReady())
        return harness
    }

    private enum CharacterCode {
        static let a = Int(Character("a").asciiValue!)
        static let c = Int(Character("c").asciiValue!)
        static let s = Int(Character("s").asciiValue!)
        static let p = Int(Character("p").asciiValue!)
        static let u = Int(Character("u").asciiValue!)
        static let comma = Int(Character(",").asciiValue!)
        static let asterisk = Int(Character("*").asciiValue!)
        static let apostrophe = Int(Character("'").asciiValue!)
        static let space = 32
        static let backspace = 8
    }
}
