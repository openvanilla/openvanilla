import Foundation
import InputMethodKit
import OpenVanillaImp
import OVModuleLoader
import OVModuleManager
import Tooltip


class InputMethodController : IMKInputController {
    private var _composingText = OpenVanilla.OVTextBufferImpl()
    private var _readingText = OpenVanilla.OVTextBufferImpl()
    private var _inputMethodContext = OpenVanilla.OVEventHandlingContext()
    private var _associatedPhrasesContext = OpenVanilla.OVEventHandlingContext()
    private var _associatedPhrasesContextInUse: Bool = false
    private var _currentClient: Any? = nil

    deinit {
    }

    override init!(server: IMKServer!, delegate: Any!, client inputClient: Any!) {
        super.init(server: server, delegate: delegate, client: inputClient)
    }

}
