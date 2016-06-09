#!/usr/bin/python

import matplotlib
import thread, time, sys, traceback, math
import numpy as np
import matplotlib
matplotlib.rcParams['backend'] = "GTKAgg"
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import regulator

#---------- SETTINGS --------------
com_port_lidar = "/dev/ttyUSB1"  # example: 5 == "COM6" == "/dev/tty5"
com_port_compass = "/dev/ttyACM0"  # example: 5 == "COM6" == "/dev/tty5"

baudrate_lidar = 115200
baudrate_compass = 57600
#---------------------------------
# serial port
ser = None
#---------------------------------
ignore_badflag = False

#---------------------------------
x=[None]*360
y=[None]*360
timePt=[None]*360
init_level = 0
index = 0

def parsedata( angle, data, x ,y):
    # unpack data using the denomination used during the discussions
    x0 = data[0]
    x1 = data[1]
    x2 = data[2]
    x3 = data[3]

    dist_mm = x0 | ((x1 & 0x3f) << 8)  # distance is coded on 13 bits ? 14 bits ?
    quality = x2 | (x3 << 8)  # quality is on 16 bits

    # display the sample
    if x1 & 0x80: # is the flag for "bad data" set?
        ok = 0
#        print "Bad data at " + str(angle) + " speed too high"
    else:
        if not x1 & 0x40:
 #           print "Good data at " + str(angle) + " speed to high"
            addPoint(dist_mm, angle, x, y)
        else:
            ok = -1

def checksum(data):
    """Compute and return the checksum as an int.

    data -- list of 20 bytes (as ints), in the order they arrived in.
    """
    # group the data by word, little-endian
    data_list = []
    for t in range(10):
        data_list.append( data[2*t] + (data[2*t+1]<<8) )

    # compute the checksum on 32 bits
    chk32 = 0
    for d in data_list:
        chk32 = (chk32 << 1) + d

    # return a value wrapped around on 15bits, and truncated to still fit into 15 bits
    checksum = (chk32 & 0x7FFF) + ( chk32 >> 15 ) # wrap around to fit into 15 bits
    checksum = checksum & 0x7FFF # truncate to 15 bits
    return int( checksum )

def getDist(lsb, msb):
    if (msb == "80" or msb == "70"):
        return False
    return int(msb + lsb, 16)/1000.

def addPoint(dist, theta, x, y):
    dist_x = dist*np.cos((theta+90) * np.pi/180) /10 # in cm
    dist_y = dist*np.sin((theta+90) * np.pi/180) /10 # in cm

    maxlimit = 100 # Max. Distance measured
    minlimit = 0.5 # Min. Distance measured
  #  if (theta) > 270 or (theta) < 90:
    if (dist_x < maxlimit) and (dist_x > -maxlimit) and (dist_y < maxlimit) and (dist_y > -maxlimit):
        if ((dist_x > minlimit) or (dist_x < -minlimit)) and ((dist_y > minlimit) or (dist_y < -minlimit)):
            x[theta] = dist_x
            y[theta] = dist_y
            timePt[theta] = 3

def gui_update_speed(speed_rpm):
    print str(speed_rpm)

def compute_speed(data):
    speed_rpm = float( data[0] | (data[1] << 8) ) / 64.0
    return speed_rpm

def read_v_2_4():
    global init_level, angle, index

    nb_errors = 0
    while True:
        try:
  #          print "getting data from lidar"
            time.sleep(0.1) # do not hog the processor power
            if init_level == 0 :
                b = ord(ser.read(1))
                # start byte
                if b == 0xFA :
                    init_level = 1
                else:
                    init_level = 0
            elif init_level == 1:
                # position index
                b = ord(ser.read(1))
                if b >= 0xA0 and b <= 0xF9  :
                    index = b - 0xA0
                    init_level = 2
                elif b != 0xFA:
                    init_level = 0
            elif init_level == 2 :
                # speed
                b_speed = [ ord(b) for b in ser.read(2)]

                # data
                b_data0 = [ ord(b) for b in ser.read(4)]
                b_data1 = [ ord(b) for b in ser.read(4)]
                b_data2 = [ ord(b) for b in ser.read(4)]
                b_data3 = [ ord(b) for b in ser.read(4)]

                # for the checksum, we need all the data of the packet...
                # this could be collected in a more elegent fashion...
                all_data = [ 0xFA, index+0xA0 ] + b_speed + b_data0 + b_data1 + b_data2 + b_data3

                # checksum
                b_checksum = [ ord(b) for b in ser.read(2) ]
                incoming_checksum = int(b_checksum[0]) + (int(b_checksum[1]) << 8)

                parsedata(index * 4 + 0, b_data0, x, y)
                parsedata(index * 4 + 1, b_data1, x, y)
                parsedata(index * 4 + 2, b_data2, x, y)
                parsedata(index * 4 + 3, b_data3, x, y)

                init_level = 0
            else: # default, should never happen...
                init_level = 0
        except :
            traceback.print_exc(file=sys.stdout)

def read_compass():
    nb_errors = 0
    while True:
        try:
            regulator.busola = int(ser_compass.readline().strip('\0'))
	    print "busola " + str(regulator.busola)
        except :
            traceback.print_exc(file=sys.stdout)

def timeout():
    for i in range(0,360):
        if timePt[i] is not None:
            timePt[i] -= 3
            if timePt[i] < 0:
                x[i] = None
                y[i] = None



plt.close('all')

# Four axes, returned as a 2-d array
f, axarr = plt.subplots(2, 2)
fig = plt.gcf()
fig.canvas.set_window_title('FRE 2016')
f.suptitle('Banat Robot')
axarr[0, 0].set_title('Stare')
axarr[0, 1].set_title('Motoare')
axarr[1, 0].set_title('Busola')
axarr[1, 1].set_title('Lidar')
# Fine-tune figure; hide x ticks for top plots and y ticks for right plots
#plt.setp([a.get_xticklabels() for a in axarr[0, :]], visible=False)
#plt.setp([a.get_yticklabels() for a in axarr[:, 1]], visible=False)

plt.ion()

time.sleep(5)
def updateplot():
    while True:
	print "updatingplot"
        axarr[1, 1].cla()
        axarr[1, 1].grid()
        timeout()
        plt.xlim(-100,100)
        plt.ylim(-100,100)
        axarr[1, 1].autoscale(enable=False)
        axarr[1, 1].scatter(x,y)
        axarr[0, 0].cla()
        axarr[0, 0].text(0.5, 0.5, str(regulator.stare), style='italic', fontsize=35,
                verticalalignment='center', horizontalalignment='center',
                bbox={'facecolor': 'green', 'pad': 10})

        axarr[0, 1].cla()
        axarr[0, 1].text(0.5, 0.7, 'Motor Stanga: ' + str(regulator.left_speed), style='italic', fontsize=20,
                         verticalalignment='center', horizontalalignment='center', color= 'blue')
        axarr[0, 1].text(0.5, 0.2, 'Motor Dreapta: ' + str(regulator.right_speed), style='italic', fontsize=20,
                         verticalalignment='center', horizontalalignment='center', color= 'blue')

        axarr[1, 0].cla()
        axarr[1, 0].text(0.5, 0.5, 'Rotatie: ' + str(regulator.busola), style='italic', fontsize=25,
                         verticalalignment='center', horizontalalignment='center', color= 'red')

        #f.savefig('lidar.png')
        plt.draw()
        plt.pause(0.1)


import serial
import os.path

if os.path.exists(com_port_lidar):
    ser = serial.Serial(com_port_lidar, baudrate_lidar)
    th = thread.start_new_thread(read_v_2_4, ())

if os.path.exists(com_port_compass):
    ser_compass = serial.Serial(com_port_compass, baudrate_compass)
    th3 = thread.start_new_thread(read_compass, ())

th2 = thread.start_new_thread(updateplot, ())

while True:
    time.sleep(0.1)
    regulator.test(x,y)
