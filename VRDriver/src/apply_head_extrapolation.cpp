static float g_lastHeadTransform[16] = {0};
static float g_lastHeadVel[3] = {0,0,0};
static float g_lastHeadAngularVel[3] = {0,0,0};
static double g_lastHeadVisionTimestamp = 0.0;
static auto g_lastHeadArrivalMacTime = std::chrono::high_resolution_clock::now();

void ApplyHeadExtrapolation(vr::TrackedDevicePose_t* pose, float* targetTransform, double visionTimestamp, float fPredictedSecondsToPhotonsFromNow) {
    auto now = std::chrono::high_resolution_clock::now();
    
    // Initialize on first frame
    if (g_lastHeadTransform[0] == 0.0f && g_lastHeadTransform[5] == 0.0f) {
        for(int i=0; i<16; i++) g_lastHeadTransform[i] = targetTransform[i];
        g_lastHeadVisionTimestamp = visionTimestamp;
        g_lastHeadArrivalMacTime = now;
    }
    
    // New UDP packet arrived
    if (visionTimestamp > g_lastHeadVisionTimestamp) {
        float dt = (float)(visionTimestamp - g_lastHeadVisionTimestamp);
        if (dt < 0.001f) dt = 0.001f;
        if (dt > 0.1f) dt = 0.1f; // Cap dt to avoid huge spikes
        
        // Calculate Velocity
        float rawVel[3];
        rawVel[0] = (targetTransform[12] - g_lastHeadTransform[12]) / dt;
        rawVel[1] = (targetTransform[13] - g_lastHeadTransform[13]) / dt;
        rawVel[2] = (targetTransform[14] - g_lastHeadTransform[14]) / dt;
        
        float velSmooth = 10.0f * dt;
        if (velSmooth > 1.0f) velSmooth = 1.0f;
        g_lastHeadVel[0] += (rawVel[0] - g_lastHeadVel[0]) * velSmooth;
        g_lastHeadVel[1] += (rawVel[1] - g_lastHeadVel[1]) * velSmooth;
        g_lastHeadVel[2] += (rawVel[2] - g_lastHeadVel[2]) * velSmooth;
        
        // Calculate Angular Velocity
        float R_prev[3][3];
        float R_cur[3][3];
        for(int r=0; r<3; r++) {
            for(int c=0; c<3; c++) {
                R_prev[r][c] = g_lastHeadTransform[c*4 + r];
                R_cur[r][c] = targetTransform[c*4 + r];
            }
        }
        
        float R_delta[3][3];
        for(int r=0; r<3; r++) {
            for(int c=0; c<3; c++) {
                R_delta[r][c] = 0;
                for(int k=0; k<3; k++) {
                    R_delta[r][c] += R_cur[r][k] * R_prev[c][k];
                }
            }
        }
        
        float rawOmegaX = (R_delta[2][1] - R_delta[1][2]) / (2.0f * dt);
        float rawOmegaY = (R_delta[0][2] - R_delta[2][0]) / (2.0f * dt);
        float rawOmegaZ = (R_delta[1][0] - R_delta[0][1]) / (2.0f * dt);
        
        if (fabs(rawOmegaX) > 50.0f) rawOmegaX = 0;
        if (fabs(rawOmegaY) > 50.0f) rawOmegaY = 0;
        if (fabs(rawOmegaZ) > 50.0f) rawOmegaZ = 0;
        
        float angSmooth = 15.0f * dt;
        if (angSmooth > 1.0f) angSmooth = 1.0f;
        g_lastHeadAngularVel[0] += (rawOmegaX - g_lastHeadAngularVel[0]) * angSmooth;
        g_lastHeadAngularVel[1] += (rawOmegaY - g_lastHeadAngularVel[1]) * angSmooth;
        g_lastHeadAngularVel[2] += (rawOmegaZ - g_lastHeadAngularVel[2]) * angSmooth;
        
        for(int i=0; i<16; i++) g_lastHeadTransform[i] = targetTransform[i];
        g_lastHeadVisionTimestamp = visionTimestamp;
        g_lastHeadArrivalMacTime = now;
    }
    
    pose->bPoseIsValid = true;
    pose->bDeviceIsConnected = true;
    pose->eTrackingResult = vr::TrackingResult_Running_OK;
    
    // Copy base transform
    for(int r=0; r<3; r++) {
        for(int c=0; c<4; c++) {
            pose->mDeviceToAbsoluteTracking.m[r][c] = targetTransform[c*4 + r];
        }
    }
    
    // Calculate total extrapolation time
    float mac_dt = std::chrono::duration<float>(now - g_lastHeadArrivalMacTime).count();
    float total_extrapolate = mac_dt + fPredictedSecondsToPhotonsFromNow;
    
    // Cap extrapolation to avoid motion sickness flying off
    if (total_extrapolate > 0.030f) total_extrapolate = 0.030f;
    if (total_extrapolate < 0.0f) total_extrapolate = 0.0f;
    
    // Extrapolate Position
    pose->mDeviceToAbsoluteTracking.m[0][3] += g_lastHeadVel[0] * total_extrapolate;
    pose->mDeviceToAbsoluteTracking.m[1][3] += g_lastHeadVel[1] * total_extrapolate;
    pose->mDeviceToAbsoluteTracking.m[2][3] += g_lastHeadVel[2] * total_extrapolate;
    
    // Extrapolate Rotation (Small Angle Approximation)
    float wx = g_lastHeadAngularVel[0] * total_extrapolate;
    float wy = g_lastHeadAngularVel[1] * total_extrapolate;
    float wz = g_lastHeadAngularVel[2] * total_extrapolate;
    
    float R_delta_extrapolate[3][3] = {
        { 1.0f, -wz, wy },
        { wz, 1.0f, -wx },
        { -wy, wx, 1.0f }
    };
    
    float currentR[3][3];
    for(int r=0; r<3; r++) {
        for(int c=0; c<3; c++) {
            currentR[r][c] = pose->mDeviceToAbsoluteTracking.m[r][c];
        }
    }
    
    for(int r=0; r<3; r++) {
        for(int c=0; c<3; c++) {
            pose->mDeviceToAbsoluteTracking.m[r][c] = 0;
            for(int k=0; k<3; k++) {
                pose->mDeviceToAbsoluteTracking.m[r][c] += R_delta_extrapolate[r][k] * currentR[k][c];
            }
        }
    }
    
    // Re-normalize rotation matrix to prevent distortion
    float col0_mag = sqrt(pose->mDeviceToAbsoluteTracking.m[0][0]*pose->mDeviceToAbsoluteTracking.m[0][0] + pose->mDeviceToAbsoluteTracking.m[1][0]*pose->mDeviceToAbsoluteTracking.m[1][0] + pose->mDeviceToAbsoluteTracking.m[2][0]*pose->mDeviceToAbsoluteTracking.m[2][0]);
    float col1_mag = sqrt(pose->mDeviceToAbsoluteTracking.m[0][1]*pose->mDeviceToAbsoluteTracking.m[0][1] + pose->mDeviceToAbsoluteTracking.m[1][1]*pose->mDeviceToAbsoluteTracking.m[1][1] + pose->mDeviceToAbsoluteTracking.m[2][1]*pose->mDeviceToAbsoluteTracking.m[2][1]);
    float col2_mag = sqrt(pose->mDeviceToAbsoluteTracking.m[0][2]*pose->mDeviceToAbsoluteTracking.m[0][2] + pose->mDeviceToAbsoluteTracking.m[1][2]*pose->mDeviceToAbsoluteTracking.m[1][2] + pose->mDeviceToAbsoluteTracking.m[2][2]*pose->mDeviceToAbsoluteTracking.m[2][2]);
    
    if (col0_mag > 0) { pose->mDeviceToAbsoluteTracking.m[0][0]/=col0_mag; pose->mDeviceToAbsoluteTracking.m[1][0]/=col0_mag; pose->mDeviceToAbsoluteTracking.m[2][0]/=col0_mag; }
    if (col1_mag > 0) { pose->mDeviceToAbsoluteTracking.m[0][1]/=col1_mag; pose->mDeviceToAbsoluteTracking.m[1][1]/=col1_mag; pose->mDeviceToAbsoluteTracking.m[2][1]/=col1_mag; }
    if (col2_mag > 0) { pose->mDeviceToAbsoluteTracking.m[0][2]/=col2_mag; pose->mDeviceToAbsoluteTracking.m[1][2]/=col2_mag; pose->mDeviceToAbsoluteTracking.m[2][2]/=col2_mag; }
    
    pose->vVelocity.v[0] = g_lastHeadVel[0];
    pose->vVelocity.v[1] = g_lastHeadVel[1];
    pose->vVelocity.v[2] = g_lastHeadVel[2];
    pose->vAngularVelocity.v[0] = g_lastHeadAngularVel[0];
    pose->vAngularVelocity.v[1] = g_lastHeadAngularVel[1];
    pose->vAngularVelocity.v[2] = g_lastHeadAngularVel[2];
}
