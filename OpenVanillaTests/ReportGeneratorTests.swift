import Testing
@testable import OpenVanilla

struct OpenVanillaTests {

    @Test func testGenerate() async throws {
        let report = await ReportGenerator.generate()
        #expect(!report.isEmpty)
//        print(report)
    }

}
