import struct
import time
import os

# The struct is:
# uint32 buttonsLeft
# uint32 buttonsRight
# float thumbstickLeftX, thumbstickLeftY
# float thumbstickRightX, thumbstickRightY
# float rotLeftW, X, Y, Z
# float rotRightW, X, Y, Z
# Total = 4*2 + 4*4 + 4*4 + 4*4 = 8 + 16 + 16 + 16 = 56 bytes

FILE_PATH = "/Users/motonishikoudai/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_input.dat"

def create_file():
    if not os.path.exists(FILE_PATH):
        with open(FILE_PATH, "wb") as f:
            f.write(b'\x00' * 56)

def write_input(btn_left, btn_right, tlx, tly, trx, try_, rot):
    with open(FILE_PATH, "r+b") as f:
        data = struct.pack("<IIffffffffffff", 
            btn_left, btn_right, 
            tlx, tly, trx, try_,
            rot[0], rot[1], rot[2], rot[3],
            rot[0], rot[1], rot[2], rot[3]
        )
        f.write(data)

if __name__ == "__main__":
    create_file()
    print("Writing fake inputs to vr_shared_input.dat... Press Ctrl+C to stop.")
    
    try:
        while True:
            # Fake a 'Grab' or 'Fire' button press every second
            print("Pressing button 1 (Grab/Fire)")
            write_input(1, 1, 0.0, 0.0, 0.0, 0.0, [1.0, 0.0, 0.0, 0.0])
            time.sleep(1)
            
            print("Releasing button")
            write_input(0, 0, 0.0, 0.0, 0.0, 0.0, [1.0, 0.0, 0.0, 0.0])
            time.sleep(1)
    except KeyboardInterrupt:
        print("Done.")
