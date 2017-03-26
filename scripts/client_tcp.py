#!/usr/bin/python
import socket
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", required=True, type=int)
    parser.add_argument("address")
    args = parser.parse_args()

    print "Connecting to server..."
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((args.address, args.port))

    print "Sending message"
    sock.sendall("ryan sucks")
    print "Done!"
    sock.close()
