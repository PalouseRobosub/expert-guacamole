from datetime import datetime
from texter import Texter
import requests
import socket
import threading
import twython

class Server():
    def __init__(self, port = 5959, twitter_key_file='twitter.key', mailgun_key='mailgun.key'):
        # Set the default parameters of the informer.
        self.phone_number = '1234567890'
        self.email_address = 'dave@example.com'
        self.hashtag = '#MailsHere'
        self.send_tweet = True
        self.send_text = False
        self.send_email = False

        # Set up the Twilio messanger, Twitter API, and the MailGun API.
        self.messager = Texter()
        twitter_keys = open('twitter.key').readlines()
        TWITTER_APP_KEY = twitter_keys[0].rstrip('\n')
        TWITTER_APP_SECRET = twitter_keys[1].rstrip('\n')
        TWITTER_OAUTH_TOKEN = twitter_keys[2].rstrip('\n')
        TWITTER_OAUTH_TOKEN_SECRET = twitter_keys[3].rstrip('\n')
        self.twitter = twython.Twython(TWITTER_APP_KEY,
                                       TWITTER_APP_SECRET,
                                       TWITTER_OAUTH_TOKEN,
                                       TWITTER_OAUTH_TOKEN_SECRET)

        mailgun_keys = open('mailgun.key').readlines()
        self.mailgun_api_key = mailgun_keys[0].rstrip('\n')
        self.mailgun_url = mailgun_keys[1].rstrip('\n')

        # Set up the TCP/IP server to communicate with the module.
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind(('',5959))
        self.sock.listen(5)

        # Set up the socket reading thread.
        self.exit = False
        self.thread = threading.Thread(target=self.check_socket)
        self.thread.daemon = True
        self.thread.start()
        return

    def kill_thread(self):
        self.exit = True
        return

    def tweet(self):
        try:
            now = datetime.now();
            self.twitter.update_status(status='You\'ve got mail! {}/{} at {}:{} {}'.format(now.month, now.day, now.hour, now.minute, self.hashtag))
        except twython.TwythonError:
            print('Failed to tweet. Did you post a duplicate?')
        return

    def text(self):
        self.messager.send(self.phone_number, 'Your mail has arrived!')
        return

    def email(self):
        requests.post(self.mailgun_url, auth=('api', self.mailgun_api_key),
                data={
                'from': 'Expert guacamole <ExpertGuacamole@smartmail.com>',
                'to': self.email_address,
                'subject': 'You\'ve got Mail!',
                'text': 'Mail has just arrived at your mailbox.'})
        return

    def check_socket(self):
        while not self.exit:
            client, address = self.sock.accept()
            msg = client.recv(1024)
            client.close()

            print ('Packet ({}): {}'.format(address, msg))

            # TODO: Verify that the message is legit here.
            is_legit = True

            if is_legit is True:
                print('Data is legit.')
                if self.send_tweet is True:
                    print('Tweeting with {}'.format(self.hashtag))
                    self.tweet()
                if self.send_text is True:
                    print('Texting {}'.format(self.phone_number))
                    self.text()
                if self.send_email is True:
                    print('Emailing {}'.format(self.email_address))
                    self.email()
        print('Server socket thread has died.')
        return

    def update_parameters(self, phone_number, email_address, hashtag):
        self.send_text = False
        self.send_email = False
        self.send_tweet = False
        if phone_number is not None:
            self.phone_number = phone_number.strip('() -')
            self.send_text = True

        if email_address is not None:
            self.email_address = email_address
            self.send_email = True

        if hashtag is not None:
            self.hashtag = '#' + hastag.lstrip('#')
            self.send_tweet = True
        return
