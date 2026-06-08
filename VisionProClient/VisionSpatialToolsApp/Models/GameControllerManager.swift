import Foundation
import GameController
import Combine

class GameControllerManager: ObservableObject {
    @Published var leftButtons: UInt32 = 0
    @Published var rightButtons: UInt32 = 0
    @Published var leftStickX: Float = 0.0
    @Published var leftStickY: Float = 0.0
    @Published var rightStickX: Float = 0.0
    @Published var rightStickY: Float = 0.0
    
    // Right Buttons Bitmask
    let BTN_A: UInt32 = 1 << 0
    let BTN_B: UInt32 = 1 << 1
    let BTN_ZR: UInt32 = 1 << 2
    let BTN_R3: UInt32 = 1 << 3
    let BTN_PLUS: UInt32 = 1 << 4
    let BTN_R_SL: UInt32 = 1 << 5
    let BTN_R_SR: UInt32 = 1 << 6
    
    // Left Buttons Bitmask
    let BTN_DPAD_UP: UInt32 = 1 << 0
    let BTN_DPAD_DOWN: UInt32 = 1 << 1
    let BTN_DPAD_LEFT: UInt32 = 1 << 2
    let BTN_DPAD_RIGHT: UInt32 = 1 << 3
    let BTN_ZL: UInt32 = 1 << 4
    let BTN_L3: UInt32 = 1 << 5
    let BTN_MINUS: UInt32 = 1 << 6
    let BTN_L_SL: UInt32 = 1 << 7
    let BTN_L_SR: UInt32 = 1 << 8
    
    private var observers: Set<AnyCancellable> = []
    
    init() {
        NotificationCenter.default.publisher(for: .GCControllerDidConnect)
            .sink { [weak self] notification in
                guard let controller = notification.object as? GCController else { return }
                print("GameController connected: \(controller.vendorName ?? "Unknown")")
                self?.setupController(controller)
            }
            .store(in: &observers)
            
        NotificationCenter.default.publisher(for: .GCControllerDidDisconnect)
            .sink { notification in
                guard let controller = notification.object as? GCController else { return }
                print("GameController disconnected: \(controller.vendorName ?? "Unknown")")
            }
            .store(in: &observers)
            
        // Initial check for already connected controllers
        for controller in GCController.controllers() {
            setupController(controller)
        }
    }
    
    private func setupController(_ controller: GCController) {
        guard let gamepad = controller.extendedGamepad else { return }
        
        gamepad.valueChangedHandler = { [weak self] (gamepad, element) in
            self?.updateState(gamepad: gamepad)
        }
    }
    
    private func updateState(gamepad: GCExtendedGamepad) {
        var rBtn: UInt32 = 0
        var lBtn: UInt32 = 0
        
        // Right Side
        if gamepad.buttonA.isPressed { rBtn |= BTN_A }
        if gamepad.buttonB.isPressed { rBtn |= BTN_B }
        if gamepad.rightTrigger.isPressed || gamepad.rightTrigger.value > 0.5 { rBtn |= BTN_ZR }
        if gamepad.rightThumbstickButton?.isPressed == true { rBtn |= BTN_R3 }
        if gamepad.buttonMenu.isPressed { rBtn |= BTN_PLUS }
        
        // Map right shoulder (bumper) to R_SL for grip
        if gamepad.rightShoulder.isPressed { rBtn |= BTN_R_SL }
        
        // Left Side
        if gamepad.dpad.up.isPressed { lBtn |= BTN_DPAD_UP }
        if gamepad.dpad.down.isPressed { lBtn |= BTN_DPAD_DOWN }
        if gamepad.dpad.left.isPressed { lBtn |= BTN_DPAD_LEFT }
        if gamepad.dpad.right.isPressed { lBtn |= BTN_DPAD_RIGHT }
        if gamepad.leftTrigger.isPressed || gamepad.leftTrigger.value > 0.5 { lBtn |= BTN_ZL }
        if gamepad.leftThumbstickButton?.isPressed == true { lBtn |= BTN_L3 }
        if gamepad.buttonOptions?.isPressed == true { lBtn |= BTN_MINUS }
        
        // Map left shoulder (bumper) to L_SL for grip
        if gamepad.leftShoulder.isPressed { lBtn |= BTN_L_SL }
        
        // Sticks
        let lX = gamepad.leftThumbstick.xAxis.value
        let lY = gamepad.leftThumbstick.yAxis.value
        let rX = gamepad.rightThumbstick.xAxis.value
        let rY = gamepad.rightThumbstick.yAxis.value
        
        DispatchQueue.main.async {
            self.rightButtons = rBtn
            self.leftButtons = lBtn
            self.leftStickX = lX
            self.leftStickY = lY
            self.rightStickX = rX
            self.rightStickY = rY
        }
    }
}
