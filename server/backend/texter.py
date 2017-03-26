#!/usr/bin/python
from twilio.rest import TwilioRestClient

class Texter():
    def __init__(self):

        # load up private information from twilio.key file
        try:
            keyfile = open("twilio.key", "r")
        except IOError:
            print "could not keyfile \"twilio.key\""
            exit(1)
        lines = keyfile.readlines()
        keyfile.close()
        self.sid = lines[0].rstrip("\n")
        self.token = lines[1].rstrip("\n")
        self.my_number = lines[2].rstrip("\n")

        # create client for sending messages
        self.client = TwilioRestClient(self.sid, self.token)

    # the return of this function is just meant for debugging purposes, don't
    # use it to tell if the function succeeded or not
    def send(self, number, message):
        response = self.client.messages.create(to=number, from_=self.my_number,
                                               body=message)
        return response



if __name__ == "__main__":
    pass
