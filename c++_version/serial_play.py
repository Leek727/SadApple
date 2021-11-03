from types import FrameType
import serial
import time
from os import walk
import gc

parent_dir = "bad_apple"
fps = 30


blank = "16384b"

print("Looking for a device...")
while True:
    try:
        port = serial.Serial("/dev/ttyACM0", baudrate=9600, timeout=0)
        print("Connected!")
        break

    except:
        time.sleep(1)

def write_port(data):
    #start = time.time()
    port.write(data.encode())
    #stop = time.time() - start

    #frame_time = round((1/fps), 3)

    #frame_time = frame_time - stop
 
    time.sleep(.023)
    

    #if frame_time > 0:
    #    time.sleep(frame_time)
    
def num_frames(parent_dir):
    _, _, filenames = next(walk(parent_dir))
    largest = 0
    for x in filenames:
        x = int(x[:-4])
        if x > largest:
            largest = x

    return largest

import subprocess
subprocess.run("xdg-open bad_apple.mp4".split(" "))

time.sleep(.06)
disconnected = False
while True:
    #for i in range(1, num_frames(parent_dir)):
    
    start = time.time()
    i = 1
    while i < num_frames(parent_dir):
        data = b""
        with open(f"{parent_dir}/{str(i)}.txt", "r") as f:
            data = f.read()

        data += "\n"

        try:
            if disconnected:
                write_port(blank)
            else:
                write_port(data)    
            i += 1
        
        except KeyboardInterrupt:
            exit(0)

        except Exception as e:
            print(e)
            port.close()
            disconnected = True
            try:
                port = serial.Serial("/dev/ttyACM0", baudrate=9600, timeout=0)
                print("Reconnected!")
                disconnected = False

            except serial.serialutil.SerialException:
                pass
    stop = time.time() - start
    print(stop)
    gc.collect()
