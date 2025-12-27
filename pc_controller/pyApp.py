#! /usr/bin/python
import serial
import time
import sys
import csv
import re
import socket


colorChronDict = {}





class SocketSerialAdapter:
    def __init__(self, ip, port, timeout=2):
        self.ip = ip
        self.port = int(port)
        self.timeout = timeout
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.settimeout(timeout)
        try:
            self.sock.connect((self.ip, self.port))
            print(f"Connected to {self.ip}:{self.port}")
        except Exception as e:
            print(f"Failed to connect to {self.ip}:{self.port} - {e}")
            sys.exit(1)

    def write(self, data):
        try:
            self.sock.sendall(data)
        except Exception as e:
            print(f"Socket write error: {e}")

    def read(self, size=1):
        try:
            data = self.sock.recv(size)
            return data
        except socket.timeout:
            return b''
        except Exception as e:
            print(f"Socket read error: {e}")
            return b''

    def reset_input_buffer(self):
        # Sockets don't have a direct equivalent, but we can drain the buffer
        try:
            self.sock.setblocking(0)
            while self.sock.recv(1024): pass
        except:
            pass
        self.sock.settimeout(self.timeout)

    def reset_output_buffer(self):
        pass # Not easily doable/needed for sockets in this context

    def close(self):
        self.sock.close()


def startSerPort(port_or_ip, baud_or_port=115200, is_tcp=False):
    if is_tcp:
        print(f"Starting TCP connection to {port_or_ip}:{baud_or_port}")
        return SocketSerialAdapter(port_or_ip, baud_or_port)
    else:
        #921600 
        print
        # ser = serial.Serial('/dev/ttyS0', 115200, timeout=1, bytesize=8)
        ser = serial.Serial(port_or_ip, baud_or_port, timeout=2, bytesize=8)
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        print (ser)
        return ser
            
# Defining main function
def main()->int:
    print("This is the main program")
    
    is_tcp = False
    port_arg = "COM7" # Default
    baud_arg = 115200

    if len(sys.argv) > 1:
        if sys.argv[1] == "--tcp":
            is_tcp = True
            if len(sys.argv) > 2:
                port_arg = sys.argv[2] # IP Address
            if len(sys.argv) > 3:
                baud_arg = int(sys.argv[3]) # Port number
            else:
                baud_arg = 8080 # Default port for Pi app
        else:
            port_arg = sys.argv[1]
            if len(sys.argv) > 2:
                baud_arg = sys.argv[2]

    ser = startSerPort(port_arg, baud_arg, is_tcp)
    while(True):
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        print("Commands:")
        print("on " + str(port_arg))
        # print("g: get brightness")
        print("l: cLear colorMap")
        print("m: send colorMap")
        print("f: toggle anti flicker")
        print("c: Clock mode")
        print("w: stopWatch mode")
        print("u: Update RTC")
        print("b: set Brightness")
        print("d: start the countDown")
        print("s: Start the stopwatch")
        print("t: stop the sTopwatch")
        print("p: Lap or Pause the stopwatch")
        print("r: Reset the stopwatch")
        print("q: quit")
        resp = input("Enter a command: ")
        # if(resp == "g"):
        #     ser.write("g000F\n".encode())
        #     getResponse(ser)
        if(resp == "l"):
            msg="l000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp == "m"):
            sendColorMap(ser,"toSend.csv")
        elif(resp == "f"):
            msg="f000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp == "u"):
            setTime(ser)
        elif(resp == "w"):
            msg = "w000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp == "c"):
            msg="c000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp == "b"):
            resp = input("Enter a brightness: ")
            if(resp[0] == 'd' or resp[0] == 'u'):
                msg="b002 "+resp+"F\n"
                msg=bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8')
                ser.write(msg)
            else:
                msg = "b002 {:02n}F\n".format(int(resp))
                msg = bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8')
                ser.write(msg)
            getResponse(ser)
        elif(resp=="d"):
            resp = input("Enter a countdown:  ")
            if(resp == ''):
                resp = '00:00:00:00.000'
            msg="d015 "+ resp+"F\n"
            msg = bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8')
            print(msg)
            ser.write(msg)
            getResponse(ser)
        elif(resp=="s"):
            msg="s000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp=="t"):
            msg="t000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp=="p"):
            msg="p000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp=="r"):
            msg="r000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp=="l"):
            msg="l000 F\n"
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            getResponse(ser)
        elif(resp == "q"):
            break;
        else:
            print("Invalid command")
    return 0

def sendColorMap(ser,filename):
    # settings = ser.get_settings()
    # settings['timeout']=1
    # ser.apply_settings(settings)
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    with open(filename, newline='') as file:
        file.readline();
        for line in file:
            # print(line)
            # ser.write("m".encode())
            line = re.sub(r'\s+', '', line)
            # line = "b003200F\n"
            line = "m{:03n} {}F\n".format(len(line), line);
            # print(line)
            # time.sleep(0.01)
            print(".",end="",flush=True)
            msg=line
            # print(msg)
            ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
            resp = getResponse(ser)
            if resp != ACK:
                time.sleep(1)
                print("Retrying")
                ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
                if resp != ACK:
                    time.sleep(5)
                    print("Retrying Again")
                    ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
                    if resp != ACK:
                        print("Failed to send colorMap")
                        print(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
                        

        print("\n")

def fletcher16(data):
    sum1 = 0
    sum2 = 0
    # print(data)
    for byte in data:
        sum1 = (sum1 + byte) % 255
        sum2 = (sum2 + sum1) % 255
        # print("sum1 {:d} sum2 {:d}".format(sum1, sum2))
    # print("sum1 {:d} sum2 {:d} {:d}".format(sum1, sum2,(sum2 ^ sum1)))
    # return (sum2 << 8) | sum1
    return (sum2 ^ sum1)%255
NACK = 0x15
ACK = 0x06
def getResponse(ser):
    # print(ser.get_settings())
    rv = ser.read(1)
    if not rv: return -1 # Timeout
    rv = int.from_bytes(rv, byteorder='big')
    if rv==NACK :
        print("Error")
    elif int(rv)==ACK:
        # print("success")
        pass
    else:
        print("Unknown response")
        print(rv)
    # time.sleep(1)
    return rv




def setTime(ser):
    ser.reset_input_buffer()
    ser.reset_output_buffer()
    # settings = ser.get_settings()
    # # print(settings)
    # settings['timeout']=1
    # ser.apply_settings(settings)
    # print(ser.get_settings())
    # ser.write(bytes_obj)
    currTime = time.localtime()
    # print(currTime)
    msg = "u019f{:04n}{:02n}{:02n}{:02n}{:02n}{:02n}{:01n}{:03n}{:01n}F\n".format(currTime.tm_year,currTime.tm_mon,currTime.tm_mday,currTime.tm_hour,currTime.tm_min,currTime.tm_sec,currTime.tm_wday,currTime.tm_yday,currTime.tm_isdst)
    ser.write(bytes(msg[:4],'utf-8') + fletcher16(msg[:4].encode()).to_bytes() + bytes(msg[5:],'utf-8'))
    getResponse(ser)


    # while(True):
    #     ser.write("c".encode())
    #     time.sleep(5)
    #     ser.write("c".encode())
    #     time.sleep(5)


if __name__ == '__main__':
    print("This is the main program")
    sys.exit(main())
