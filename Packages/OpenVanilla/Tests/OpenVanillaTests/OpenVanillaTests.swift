// Copyright (c) 2022 and onwards The OpenVanilla Authors.
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

import ArrayIMTestSupport
import XCTest

// Characterization tests for the Array (行列30) module. These pin down the
// behavior of the current (legacy) engine before it is rewritten against the
// modern OpenVanilla API, so the rewrite can be verified against them. The
// behaviors follow the Array30 porting spec (行列輸入法Porting規格) as
// implemented by the current engine and the bundled DataTables.

final class ArraySpecBehaviorTests: XCTestCase {

    private var dataTablesDirectory: URL {
        URL(fileURLWithPath: #filePath)
            .deletingLastPathComponent()   // OpenVanillaTests
            .deletingLastPathComponent()   // Tests
            .deletingLastPathComponent()   // OpenVanilla
            .deletingLastPathComponent()   // Packages
            .deletingLastPathComponent()   // repository root
            .appendingPathComponent("DataTables")
    }

    // MARK: - Helpers

    private func makeSession() -> OpaquePointer? {
        ov_array_session_create(dataTablesDirectory.path)
    }

    private func typeKeys(_ session: OpaquePointer?, _ keys: String) {
        for scalar in keys.unicodeScalars {
            _ = ov_array_session_key(session, Int8(truncatingIfNeeded: scalar.value))
        }
    }

    private func pressCode(_ session: OpaquePointer?, _ code: Int32) {
        _ = ov_array_session_key(session, Int8(truncatingIfNeeded: code))
    }

    private func candidates(_ session: OpaquePointer?) -> [String] {
        (0..<ov_array_session_candidate_count(session)).map {
            String(cString: ov_array_session_candidate_at(session, $0))
        }
    }

    private func buffer(_ session: OpaquePointer?) -> String {
        String(cString: ov_array_session_buffer(session))
    }

    private func committed(_ session: OpaquePointer?) -> String {
        String(cString: ov_array_session_committed(session))
    }

    private func notify(_ session: OpaquePointer?) -> String {
        String(cString: ov_array_session_last_notify(session))
    }

    private func writeTemporaryCin(_ content: String) throws -> String {
        let path = NSTemporaryDirectory() + "ov-array30-test-\(UUID().uuidString).cin"
        try content.data(using: .utf8)!.write(to: URL(fileURLWithPath: path))
        return path
    }

    // MARK: - 按鍵回應顯示（spec: 鍵位以 數字+行 顯示，如 6-）

    func testKeyEchoUsesArrayPositionDisplay() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hg")
        XCTAssertEqual(buffer(s), "6-5-", "h=6-, g=5-")
    }

    // MARK: - 數字鍵（spec: 非選字狀態下按數字直接送出數字）

    func testDigitAtEmptyCompositionCommitsTheDigit() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "5")
        XCTAssertEqual(committed(s), "5")
        XCTAssertEqual(ov_array_session_is_composing(s), 0)
    }

    func testShiftDigitCommitsTheSymbolOnThatKey() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        _ = ov_array_session_key_with_modifiers(s, Int8(bitPattern: UInt8(ascii: "!")), 1, 0)
        XCTAssertEqual(committed(s), "!")
    }

    // Current engine quirk: shift+letter commits the lowercase letter.
    func testShiftLetterCommitsLowercaseLetter() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        _ = ov_array_session_key_with_modifiers(s, Int8(bitPattern: UInt8(ascii: "A")), 1, 0)
        XCTAssertEqual(committed(s), "a")
    }

    // Current engine quirk: capslock without shift commits lowercase.
    func testCapsLockLetterCommitsLowercase() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        _ = ov_array_session_key_with_modifiers(s, Int8(bitPattern: UInt8(ascii: "a")), 0, 1)
        XCTAssertEqual(committed(s), "a")
    }

    func testCapsLockShiftLetterCommitsUppercase() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        _ = ov_array_session_key_with_modifiers(s, Int8(bitPattern: UInt8(ascii: "a")), 1, 1)
        XCTAssertEqual(committed(s), "A")
    }

    // MARK: - 符號輸入（spec: W+數字 開符號組；v2026 新增 HG+數字）

    func testW1OpensPunctuationGroupAndDigitSelects() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "w1")
        let list = candidates(s)
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertEqual(list.count, 10, "first page of 10")
        XCTAssertTrue(list.contains("，") && list.contains("！"))

        // spec: 按數字[9]即選第9個符號「！」
        typeKeys(s, "9")
        XCTAssertEqual(committed(s), "！")
    }

    func testHgGroupsOpenSymbolWindows() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hg0")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertEqual(candidates(s).count, 10)
        XCTAssertEqual(candidates(s)[0], "⼀")

        ov_array_session_reset(s)
        typeKeys(s, "hg2")
        XCTAssertTrue(candidates(s).contains("↹") && candidates(s).contains("⌘"))
        XCTAssertEqual(buffer(s), candidates(s)[0])

        ov_array_session_reset(s)
        typeKeys(s, "hg8")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
    }

    func testHgReservedEmptyGroupShowsNothing() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // hg3–hg7 are reserved by the table spec and still empty.
        typeKeys(s, "hg3")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 0)
        XCTAssertEqual(ov_array_session_is_composing(s), 0)
        XCTAssertEqual(ov_array_session_beep_count(s), 0)
    }

    func testHg2ThenHg1SwitchesGroupAndCommitsFirstCandidate() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hg2hg1")
        XCTAssertEqual(committed(s), "↹", "first hg2 candidate committed on new code")
        XCTAssertEqual(candidates(s)[0], "☐")
    }

    // MARK: - 翻頁（spec: Space 翻下頁、循環回第一頁；PgUp/PgDn、方向鍵）

    func testSymbolGroupPagingWithSpaceAndArrows() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hg0")   // 330 symbols = 33 pages
        let firstPageFirst = candidates(s)[0]
        XCTAssertEqual(firstPageFirst, "⼀")

        typeKeys(s, " ")     // Space pages down
        XCTAssertNotEqual(candidates(s)[0], firstPageFirst)

        pressCode(s, 30)     // ovkUp
        XCTAssertEqual(candidates(s)[0], firstPageFirst)

        pressCode(s, 31)     // ovkDown
        XCTAssertNotEqual(candidates(s)[0], firstPageFirst)

        pressCode(s, 28)     // ovkLeft
        XCTAssertEqual(candidates(s)[0], firstPageFirst)
    }

    func testSymbolGroupPagingWrapsAround() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hg0")
        for _ in 0..<33 {
            typeKeys(s, " ")
        }
        XCTAssertEqual(candidates(s)[0], "⼀", "33 pages later Space wraps to page 1")
    }

    // MARK: - 控制鍵（spec: Space 完成組字、Backspace 回上一步、Esc 清除）

    func testSpaceCommitsSingleMatchDirectly() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "aa ")
        XCTAssertEqual(committed(s), "二")
        XCTAssertEqual(buffer(s), "")
    }

    func testSpaceWithNoMatchNotifiesWuCiZi() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "aaf ")
        XCTAssertEqual(notify(s), "無此字")
        XCTAssertEqual(committed(s), "")
        XCTAssertEqual(buffer(s), "")
        XCTAssertEqual(ov_array_session_is_composing(s), 0)
    }

    func testBackspaceStepsBackOneKey() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hgd")
        pressCode(s, 8)     // ovkBackspace
        XCTAssertEqual(buffer(s), "6-5-")
    }

    func testEscapeClearsEverything() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hg2")
        XCTAssertEqual(ov_array_session_is_composing(s), 1)

        pressCode(s, 27)    // ovkEsc
        XCTAssertEqual(buffer(s), "")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 0)
        XCTAssertEqual(ov_array_session_is_composing(s), 0)
    }

    // MARK: - 一級/二級簡碼（spec: 首鍵顯示10個一級簡碼、次鍵顯示二級簡碼）

    func testFirstKeyShowsLevelOneShortcodes() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "a")
        let list = candidates(s)
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertEqual(list.count, 10)
        XCTAssertEqual(list[0], "一")

        typeKeys(s, "2")    // select the 2nd shortcode
        XCTAssertEqual(committed(s), "到")
    }

    func testSecondKeyShowsLevelTwoShortcodes() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "aa")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertEqual(candidates(s)[0], "二")
    }

    func testSelectingPlaceholderCandidateNotifiesWuCiZi() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // The aa shortcode list contains ⎔ placeholders (e.g. at index 4).
        typeKeys(s, "aa")
        typeKeys(s, "4")
        XCTAssertEqual(notify(s), "無此字")
        XCTAssertEqual(committed(s), "")
    }

    // MARK: - 重碼字（spec: Space 查表、重碼顯示待選、Space/數字1 送上第一字）

    func testSpaceWithMultipleMatchesShowsCandidates() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // spec 範例：按 [I][F] 再按 Space → 1針 2羊 3并
        typeKeys(s, "if ")
        let list = candidates(s)
        XCTAssertEqual(ov_array_session_is_composing(s), 1)
        XCTAssertEqual(list[0], "針")
        XCTAssertEqual(list[1], "羊")
        XCTAssertEqual(list[2], "并")
        XCTAssertEqual(buffer(s), "針", "first candidate pre-selected")
        XCTAssertEqual(committed(s), "")
    }

    func testSecondSpaceCommitsFirstCandidate() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "if  ")
        XCTAssertEqual(committed(s), "針")
    }

    func testDigitSelectsDuplicateCandidate() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "if 2")
        XCTAssertEqual(committed(s), "羊")
    }

    // Current engine quirk: out-of-range digit is silently swallowed (the spec
    // asks for a beep). Pinned here so the rewrite can decide deliberately.
    func testOutOfRangeDigitIsSwallowedWithoutBeep() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "if 5")
        XCTAssertEqual(committed(s), "")
        XCTAssertEqual(ov_array_session_beep_count(s), 0)
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
    }

    func testNextRadicalKeyCommitsFirstDuplicateThenContinues() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // spec: 重碼字待選時繼續按下一個字根鍵 → 送上第一個重碼字
        typeKeys(s, "if a")
        XCTAssertEqual(committed(s), "針")
        XCTAssertEqual(buffer(s), "1-", "new composition with a")
    }

    // MARK: - 預選（spec: 第3鍵起顯示符合字可預視/預選，不必按 Space）

    func testThirdKeyPreviewsMatchesAndDigitPreselects() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "dai")   // 岼 丄 㞢 𠃎
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertEqual(candidates(s)[0], "岼")
        XCTAssertEqual(ov_array_session_is_composing(s), 0)

        typeKeys(s, "2")
        XCTAssertEqual(committed(s), "丄")
    }

    func testSingleMatchThirdKeyShowsCandidate() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "hgd")
        XCTAssertEqual(candidates(s), ["疗"])
    }

    // MARK: - 最長五碼（spec: 大字集最長五碼，末碼固定為 I）

    func testFifthKeyMustBeI() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "aaaa")     // 4 keys
        typeKeys(s, "a")        // 5th non-i key is ignored
        XCTAssertEqual(buffer(s), "1-1-1-1-")

        typeKeys(s, "i")        // 5th key i is accepted
        XCTAssertEqual(buffer(s), "1-1-1-1-8⇡")

        typeKeys(s, "a")        // 6th key is ignored
        XCTAssertEqual(buffer(s), "1-1-1-1-8⇡")
    }

    // MARK: - 詢問鍵（spec: ? 忽略該碼位、* 萬用）

    func testQuestionMarkWildcardFindsJin() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // spec 範例：A ? Q ? 應可查到「勁」（本表 awqb，排序後位於第4頁）
        typeKeys(s, "a?q? ")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        var found = candidates(s).contains("勁")
        var pages = 0
        while !found && pages < 6 {
            typeKeys(s, " ")
            found = candidates(s).contains("勁")
            pages += 1
        }
        XCTAssertTrue(found, "勁 should be among the a?q? results")
    }

    func testStarWildcardMatchesSuffixes() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // 目前的引擎採 glob 語意（非規格所述的亂序查詢）: aq* 符合 aq 開頭的字
        typeKeys(s, "aq* ")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertTrue(candidates(s).contains("死"))
    }

    func testWildcardKeyCancelsCandidateWindow() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "aa")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        typeKeys(s, "?")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 0)
    }

    // MARK: - 詞鍵（spec: ' 詞終結鍵；查無詞顯示「無此詞」）

    func testApostropheCommitsPhrase() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // spec 範例：PULP 明白
        typeKeys(s, "pulp'")
        XCTAssertEqual(committed(s), "明白")
    }

    func testApostropheWithoutPhraseNotifiesWuCiCi() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "aah'")
        XCTAssertEqual(notify(s), "無此詞彙")
        XCTAssertEqual(committed(s), "")
    }

    // MARK: - 其他內建行為

    func testTKeyPrecommitsDe() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, "t")
        XCTAssertEqual(buffer(s), "的")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
    }

    // MARK: - 特別碼提示與快打模式（spec: 特別碼提示、快打模式）

    func testSpecialCodePromptOnCommit() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        // 是 = pac，特別碼 pc
        typeKeys(s, "pac ")
        XCTAssertEqual(committed(s), "是")
        XCTAssertEqual(notify(s), "是: 0⇡3⇣")
    }

    func testQuickModeBlocksCharWithSpecialCode() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        ov_array_session_set_force_sp(s, 1)
        typeKeys(s, "pac ")
        XCTAssertEqual(committed(s), "", "quick mode must not commit")
        XCTAssertEqual(ov_array_session_beep_count(s), 1)
        XCTAssertEqual(notify(s), "是: 0⇡3⇣")
    }

    func testCommaCommaSpTogglesQuickMode() throws {
        guard let s = makeSession() else { throw XCTSkip("tables not found") }
        defer { ov_array_session_destroy(s) }
        ov_array_session_reset(s)

        typeKeys(s, ",,sp ")
        XCTAssertEqual(notify(s), "快打模式：啟用")
        XCTAssertEqual(ov_array_session_is_force_sp(s), 1)

        typeKeys(s, ",,sp ")
        XCTAssertEqual(notify(s), "快打模式：關閉")
        XCTAssertEqual(ov_array_session_is_force_sp(s), 0)

        typeKeys(s, "pac ")
        XCTAssertEqual(committed(s), "是", "quick mode off commits again")
    }

    // MARK: - 使用者匯入主表

    func testCustomMainTableReplacesBundledTable() throws {
        let content = """
        %gen_inp
        %ename test-array30
        %cname 測試行列30
        %encoding UTF-8
        %selkey 1234567890
        %keyname begin
        a 1-
        h 6-
        g 5-
        %keyname end
        %chardef begin
        aa 龘
        hg2 ⌘
        hg2 ⇧
        %chardef end

        """
        let path = try writeTemporaryCin(content)
        guard let s = ov_array_session_create_with_table(dataTablesDirectory.path, path) else {
            throw XCTSkip("tables not found")
        }
        defer { ov_array_session_destroy(s) }

        ov_array_session_reset(s)
        typeKeys(s, "aa ")
        XCTAssertEqual(committed(s), "龘")

        ov_array_session_reset(s)
        typeKeys(s, "hg2")
        XCTAssertEqual(candidates(s).count, 2)
        XCTAssertTrue(candidates(s).contains("⌘") && candidates(s).contains("⇧"))
        XCTAssertEqual(buffer(s), candidates(s)[0])
    }

    func testMissingCustomMainTableFallsBackToBundled() throws {
        let missingPath = NSTemporaryDirectory() + "ov-array30-missing-\(UUID().uuidString).cin"
        guard let s = ov_array_session_create_with_table(dataTablesDirectory.path, missingPath) else {
            throw XCTSkip("tables not found")
        }
        defer { ov_array_session_destroy(s) }

        ov_array_session_reset(s)
        typeKeys(s, "hg2")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertTrue(candidates(s).contains("↹"))

        ov_array_session_reset(s)
        typeKeys(s, "aa ")
        XCTAssertEqual(committed(s), "二")
    }

    func testCorruptCustomMainTableFallsBackToBundled() throws {
        let path = try writeTemporaryCin("this is not a cin file at all\n")
        guard let s = ov_array_session_create_with_table(dataTablesDirectory.path, path) else {
            throw XCTSkip("tables not found")
        }
        defer { ov_array_session_destroy(s) }

        ov_array_session_reset(s)
        typeKeys(s, "hg2")
        XCTAssertEqual(ov_array_session_candidates_shown(s), 1)
        XCTAssertTrue(candidates(s).contains("↹"))
    }
}
