import Testing
import Cocoa
@testable import OpenVanilla

struct OpenVanillaTests {

    @Test func testGenerate() async throws {
        let report = await ReportGenerator.generate()
        #expect(!report.isEmpty)
//        print(report)
    }

}


@MainActor
struct AssociatedPhrasesPreferencesTests {
    @Test func importControlsLoadFromNib() throws {
        let windowController = PreferencesWindowController(windowNibName: "preferences")
        _ = try #require(windowController.window)
        let controller = try #require(windowController.associatedPhrasesPreferencesViewController as? AssociatedPhrasesPreferencesViewController)
        controller.loadPreferences()
        #expect(controller.fieldTableStatus != nil)
        #expect(!controller.fieldTableStatus.stringValue.isEmpty)
        #expect(controller.restoreTableButton != nil)
        #expect(controller.view.subviews.filter { ($0 as? NSButton)?.action == #selector(AssociatedPhrasesPreferencesViewController.importTable(_:)) }.count == 1)
        windowController.close()
    }
}
