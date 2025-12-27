#!/usr/bin/env python3
import serial
import time

def startSerPort():
    #921600
    ser = serial.Serial('/dev/ttyS0', 115200, timeout=1,bytesize=8)
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    return ser

# Defining main function 
def main(): 
    ser = startSerPort()
    # bytes_obj = b'b\x05'
    # ser.write("s".encode())
    # print(ser.read(1))

    # ser.write(bytes_obj)
    currTime = time.localtime()
    ser.write("u".encode())
    # print(ser.read(1))
    ser.write((str(currTime.tm_year) + '\n').encode()) 
    # print(ser.read(1))
    ser.write((str(currTime.tm_mon) + '\n').encode())
    # print(ser.read(1))
    ser.write((str(currTime.tm_mday) + '\n').encode())
    # print(ser.read(1))
    ser.write((str(currTime.tm_wday) + '\n').encode())
    # print(ser.read(1))
    ser.write((str(currTime.tm_hour) + '\n').encode())
    # print(ser.read(1))
    ser.write((str(currTime.tm_min) + '\n').encode())
    # print(ser.read(1))
    ser.write((str(currTime.tm_sec) + '\n').encode())
    # print(ser.read(1))
    





    # while(True):
    #     ser.write("c".encode())
    #     time.sleep(5)
    #     ser.write("c".encode())
    #     time.sleep(5)


  
  
# Using the special variable  
# __name__ 
if __name__=="__main__": 
    main() 
