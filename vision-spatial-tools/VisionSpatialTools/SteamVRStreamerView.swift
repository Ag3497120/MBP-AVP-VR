import SwiftUI
import AVFoundation
import Combine

class VideoContainerView: UIView {
    let displayLayer: AVSampleBufferDisplayLayer
    
    init(displayLayer: AVSampleBufferDisplayLayer) {
        self.displayLayer = displayLayer
        super.init(frame: .zero)
        self.backgroundColor = .black
        self.layer.addSublayer(displayLayer)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        displayLayer.frame = self.bounds
    }
}

struct StreamerLayerContainer: UIViewRepresentable {
    let displayLayer: AVSampleBufferDisplayLayer
    
    func makeUIView(context: Context) -> VideoContainerView {
        return VideoContainerView(displayLayer: displayLayer)
    }
    
    func updateUIView(_ uiView: VideoContainerView, context: Context) {
        // bounds are handled by layoutSubviews
    }
}

struct SteamVRStreamerView: View {
    @StateObject private var receiver = VRUDPReceiver()
    @StateObject private var decoder = VRVideoDecoder()
    
    var body: some View {
        ZStack {
            StreamerLayerContainer(displayLayer: decoder.displayLayer)
                .edgesIgnoringSafeArea(.all)
            
            VStack {
                Spacer()
                Text("SteamVR Streamer Active")
                    .font(.headline)
                    .foregroundColor(.white)
                    .padding()
                    .background(Color.black.opacity(0.5))
                    .cornerRadius(10)
                    .padding(.bottom, 20)
            }
        }
        .onAppear {
            receiver.start(port: 9999)
        }
        .onDisappear {
            receiver.stop()
        }
        .onReceive(receiver.framePublisher.receive(on: DispatchQueue.main)) { frameData in
            decoder.decodeFrame(frameData)
        }
    }
}
