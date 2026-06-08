import CompositorServices
import Foundation

func test(timing: LayerRenderer.Timing) {
    let mTime = timing.presentationTime.machTime
    
    var info = mach_timebase_info()
    mach_timebase_info(&info)
    
    let seconds = Double(mTime) * Double(info.numer) / Double(info.denom) / 1_000_000_000.0
    print(seconds)
}
