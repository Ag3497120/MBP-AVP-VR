import re
with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

old_velocity = """        for(int i=1; i<3 && i<unGamePoseArrayCount; ++i) {
            float* current = (i==1) ? lastLeft : lastRight;
            float* pose = (i==1) ? pSharedHands->leftTransform : pSharedHands->rightTransform;
            if (pose && pose[15] != 0.0f) {
                pGamePoseArray[i].vVelocity.v[0] = (pose[12] - current[0]) / dt;
                pGamePoseArray[i].vVelocity.v[1] = (pose[13] - current[1]) / dt;
                pGamePoseArray[i].vVelocity.v[2] = (pose[14] - current[2]) / dt;
                current[0] = pose[12]; current[1] = pose[13]; current[2] = pose[14];
            }
        }
        lastTime = now;"""

new_velocity = """        static float lastLeftVel[3] = {0,0,0};
        static float lastRightVel[3] = {0,0,0};
        for(int i=1; i<3 && i<unGamePoseArrayCount; ++i) {
            float* current = (i==1) ? lastLeft : lastRight;
            float* lastVel = (i==1) ? lastLeftVel : lastRightVel;
            float* pose = (i==1) ? pSharedHands->leftTransform : pSharedHands->rightTransform;
            if (pose && pose[15] != 0.0f) {
                if (pose[12] != current[0] || pose[13] != current[1] || pose[14] != current[2]) {
                    // Packet updated! Compute new velocity.
                    lastVel[0] = (pose[12] - current[0]) / dt;
                    lastVel[1] = (pose[13] - current[1]) / dt;
                    lastVel[2] = (pose[14] - current[2]) / dt;
                    current[0] = pose[12]; current[1] = pose[13]; current[2] = pose[14];
                }
                // Always write the last non-zero velocity!
                pGamePoseArray[i].vVelocity.v[0] = lastVel[0];
                pGamePoseArray[i].vVelocity.v[1] = lastVel[1];
                pGamePoseArray[i].vVelocity.v[2] = lastVel[2];
            }
        }
        lastTime = now;"""

content = content.replace(old_velocity, new_velocity)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Fixed Velocity!")
