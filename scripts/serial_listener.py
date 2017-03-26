#!/usr/bin/python
import serial
import argparse

if __name__ == "__main__":
    s = serial.Serial()
    s.baudrate = 9600
    s.port = "/dev/ttyACM0"
    s.open()

    while True:
        print s.readline()


