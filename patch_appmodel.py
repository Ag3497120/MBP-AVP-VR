import re

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Models/AppModel.swift', 'r') as f:
    content = f.read()

# Insert joystick anchors above processTrackingForVR
anchors_code = """
    // Joystick neutral anchors
    private var leftJoystickNeutralAnchor: simd_float4x4? = nil
    private var rightJoystickNeutralAnchor: simd_float4x4? = nil
    
    private func processTrackingForVR() async {
"""
content = content.replace("    private func processTrackingForVR() async {", anchors_code)

# Replace getHandInputs block
old_getHandInputs = """            func getHandInputs(hand: HandAnchor?, isRight: Bool) -> (trigger: UInt8, grip: UInt8) {"""
end_getHandInputs = """            let leftInputs = getHandInputs(hand: hands.leftHand, isRight: false)"""

start_idx = content.find(old_getHandInputs)
end_idx = content.find(end_getHandInputs)

new_getHandInputs = """
            struct HandState {
                var trigger: UInt8 = 0
                var grip: UInt8 = 0
                var buttons: UInt32 = 0
                var stickX: Float = 0
                var stickY: Float = 0
            }

            func getHandInputs(hand: HandAnchor?, isRight: Bool) -> HandState {
                var state = HandState()
                guard let hand = hand, hand.isTracked, let skeleton = hand.handSkeleton else { return state }
                
                let thumb = skeleton.joint(.thumbTip)
                let index = skeleton.joint(.indexFingerTip)
                let middle = skeleton.joint(.middleFingerTip)
                let ring = skeleton.joint(.ringFingerTip)
                let pinky = skeleton.joint(.littleFingerTip)
                let wrist = skeleton.joint(.wrist)
                let palm = skeleton.joint(.middleFingerKnuckle) // use as center of palm
                
                guard thumb.isTracked && index.isTracked && middle.isTracked && ring.isTracked && pinky.isTracked && wrist.isTracked else { return state }
                
                func dist(_ a: simd_float4, _ b: simd_float4) -> Float {
                    return simd_distance(simd_make_float3(a.x, a.y, a.z), simd_make_float3(b.x, b.y, b.z))
                }
                
                let thumbPos = thumb.anchorFromJointTransform.columns.3
                let indexPos = index.anchorFromJointTransform.columns.3
                let middlePos = middle.anchorFromJointTransform.columns.3
                let ringPos = ring.anchorFromJointTransform.columns.3
                let pinkyPos = pinky.anchorFromJointTransform.columns.3
                let palmPos = palm.anchorFromJointTransform.columns.3
                
                let pinchThreshold: Float = 0.025 // 2.5cm
                
                // Trigger (Index + Thumb)
                let indexPinch = dist(thumbPos, indexPos)
                if indexPinch < pinchThreshold { state.trigger = 255 }
                
                // Grip (Middle, Ring, Pinky curled to palm)
                let gripDist = (dist(middlePos, palmPos) + dist(ringPos, palmPos) + dist(pinkyPos, palmPos)) / 3.0
                if gripDist < 0.06 { state.grip = 255 }
                
                // A Button (Thumb + Ring)
                if dist(thumbPos, ringPos) < pinchThreshold {
                    state.buttons |= isRight ? (1 << 0) : (1 << 1) // BTN_A or BTN_DPAD_DOWN
                }
                
                // B Button (Thumb + Pinky)
                if dist(thumbPos, pinkyPos) < pinchThreshold {
                    state.buttons |= isRight ? (1 << 1) : (1 << 3) // BTN_B or BTN_DPAD_RIGHT
                }
                
                // Virtual Joystick (Thumb + Middle)
                if dist(thumbPos, middlePos) < pinchThreshold {
                    let currentRot = hand.originFromAnchorTransform
                    if isRight {
                        if rightJoystickNeutralAnchor == nil { rightJoystickNeutralAnchor = currentRot }
                        if let neutral = rightJoystickNeutralAnchor {
                            // Extract relative pitch/roll
                            let relative = matrix_multiply(simd_inverse(neutral), currentRot)
                            state.stickX = max(-1.0, min(1.0, relative.columns.0.y * 3.0)) // Roll
                            state.stickY = max(-1.0, min(1.0, -relative.columns.2.y * 3.0)) // Pitch
                        }
                    } else {
                        if leftJoystickNeutralAnchor == nil { leftJoystickNeutralAnchor = currentRot }
                        if let neutral = leftJoystickNeutralAnchor {
                            let relative = matrix_multiply(simd_inverse(neutral), currentRot)
                            state.stickX = max(-1.0, min(1.0, relative.columns.0.y * 3.0)) // Roll
                            state.stickY = max(-1.0, min(1.0, -relative.columns.2.y * 3.0)) // Pitch
                        }
                    }
                } else {
                    // Reset anchor when released
                    if isRight { rightJoystickNeutralAnchor = nil }
                    else { leftJoystickNeutralAnchor = nil }
                }
                
                return state
            }
            
"""
content = content[:start_idx] + new_getHandInputs + content[end_idx:]

# Update the compositor.sendPose call
old_sendPose = """            compositor.sendPose(
                head: headT,
                leftHand: leftT,
                rightHand: rightT,
                leftPinch: leftInputs.grip,
                rightPinch: rightInputs.grip,
                leftTrigger: leftInputs.trigger,
                rightTrigger: rightInputs.trigger
            )"""

new_sendPose = """            compositor.sendPose(
                head: headT,
                leftHand: leftT,
                rightHand: rightT,
                leftPinch: leftInputs.grip,
                rightPinch: rightInputs.grip,
                leftTrigger: leftInputs.trigger,
                rightTrigger: rightInputs.trigger,
                rightButtons: rightInputs.buttons,
                leftButtons: leftInputs.buttons,
                rightStickX: rightInputs.stickX,
                rightStickY: rightInputs.stickY,
                leftStickX: leftInputs.stickX,
                leftStickY: leftInputs.stickY
            )"""
content = content.replace(old_sendPose, new_sendPose)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Models/AppModel.swift', 'w') as f:
    f.write(content)

