import cv2
import os
import time
import subprocess
import itertools

parent_dir = "bad_apple"

try:
    os.makedirs(parent_dir)
except:
    pass

def format_img(data, file_num):
    center = data.shape
    w = int(sorted(center[:-1])[0])
    h = w

    x = center[1]/2 - w/2
    y = center[0]/2 - h/2

    data = data[int(y):int(y+h), int(x):int(x+w)]

    data = cv2.resize(data,(128,128))   

    im_gray = cv2.cvtColor(data, cv2.COLOR_BGR2GRAY)


    thresh = 200
    im_bw = cv2.threshold(im_gray, thresh, 1, cv2.THRESH_BINARY)[1]

    bit_list = list(itertools.chain.from_iterable(im_bw)) #convert lists of lists to a single list
    bit_list = "".join("{0}".format(n) for n in bit_list).replace('0','b').replace('1','w') #convert int list to a string

    #printout - used in -      python3 1bit.py | ./parse
    return str(file_num) + bit_list

#vidcap = cv2.VideoCapture('Touhou - Bad Apple.mp4')
vidcap = cv2.VideoCapture('bad_apple.mp4')

success, image = vidcap.read()
count = 0

start = time.time()
while success:
    # cv2.imwrite("frames/frame%d.jpg" % count, image)     # save frame as JPEG file
    success, image = vidcap.read()
    if success:
        print(count)
        count += 1

        str_count = parent_dir + "/" + str(count)
        str_count += "-" * (20-len(str_count)) #turn 123 into "123---"

        data = format_img(image, str_count)

        subprocess.run(["./parse.sh",data])

    else:
        pass

stop = time.time() - start

print(stop)