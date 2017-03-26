#!/usr/bin/python
import zmq
import argparse
import time

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--port", required=True)
    parser.add_argument("address")
    args = parser.parse_args()

    context = zmq.Context()
    print "Connecting to server..."
    socket = context.socket(zmq.PAIR)
    socket.connect("tcp://{}:{}".format(args.address, args.port))

    print "Sending message"
    socket.send("ryan sucks")
    time.sleep(.1)
    print "Done!"
    socket.close()

