from flask import Flask
from flask import request
import requests
import twython
import sys
import threading
import zmq
import signal
import time

# Construct a Flask web server object.
app = Flask(__name__)

# Threading exit variable
exit = False

# Construct the network ports
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.bind('tcp://0.0.0.0:5959')

# Default parameters for tweeting, emailing, and sending text messages.
send_email = False
send_tweet = False
send_text = False
phone_number = '123-456-7890'
email_address = 'default@gmail.com'
hashtag = 'MailsHere'

# Set up twitter API
twitter_keys = open('twitter.key').readlines()
TWITTER_APP_KEY = twitter_keys[0].rstrip('\n')
TWITTER_APP_SECRET = twitter_keys[1].rstrip('\n')
TWITTER_OAUTH_TOKEN = twitter_keys[2].rstrip('\n')
TWITTER_OAUTH_TOKEN_SECRET = twitter_keys[3].rstrip('\n')
print TWITTER_APP_KEY

twitter = twython.Twython(TWITTER_APP_KEY, \
                  TWITTER_APP_SECRET, \
                  TWITTER_OAUTH_TOKEN, \
                  TWITTER_OAUTH_TOKEN_SECRET)

# Set up the MailGun API
mailgun_keys = open('mailgun.key').readlines()
MAILGUN_API_KEY = mailgun_keys[0].rstrip('\n')
MAILGUN_URL = mailgun_keys[1].rstrip('\n')

@app.route('/home')
def default_page():
    return 'Operation Expert Guacamole is a go.'

# Configures the server to send texts, tweet, or send emails.
#
# Available names:
#   email={True|False}
#   text={True|False}
#   tweet={True|False}
#   email_address={address}
#   phone_number={number}
#   hashtag={tag}
@app.route("/update", methods=['POST'])
def configure():
    global send_email
    global send_text
    global send_tweet
    global email_address
    global hashtag
    global phone_number

    arguments = dict(request.form)
    print('Received an update request: {}'.format(arguments))

    if 'email' in arguments and arguments['email'][0].encode('ascii', 'ignore') == 'True':
        send_email = True;
        if 'email_address' in arguments:
            print('Updating email address to {}'.format(email_address))
            email_address = arguments['email_address'][0].encode('ascii', 'ignore')
    if 'text' in arguments and arguments['text'][0].encode('ascii', 'ignore') == 'True':
        send_text = True;
        if 'phone_number' in arguments:
            phone_number = arguments['phone_number'][0].encode('ascii', 'ignore')
    if 'tweet' in arguments:
        send_tweet = True;
        if 'hashtag' in arguments:
            hashtag = arguments['hashtag'][0].encode('ascii', 'ignore')
    return ''

def tweet():
    try:
        twitter.update_status(status='You\'ve got mail! (Operation Expert Guacamole is a go) #{}'.format(hashtag))
    except TwythonError:
        print('Failed to tweet. (Duplicate?)')
    return

def text():
    #TODO Try and set up text messages.
    return

def email():
    requests.post(MAILGUN_URL, auth=('api', MAILGUN_API_KEY), data={
            'from': 'Expert guacamole <ExpertGuacamole@smartmail.com>',
            'to': email_address,
            'subject': 'You\'ve got Mail!',
            'text': 'Your mail has arrived!'})
    return

def check_socket():
    while not exit:
        try:
            msg = socket.recv()
        except zmq.ZMQError:
            continue

        print ('Packet: {}'.format(msg))

        # TODO: Verify that the message is legit here.
        is_legit = True

        if is_legit is True:
            print('Data is legit.')
            if send_tweet is True:
                print('Tweeting with #{}'.format(hashtag))
                tweet()
            if send_text is True:
                print('Texting {}'.format(phone_number))
                text()
            if send_email is True:
                print('Emailing {}'.format(email_address))
                email()
    print('Thread is dying.')
    return

# Construct the worker thread for the receiving function
thread = threading.Thread(target=check_socket)
thread.daemon = True

# CTRL+C Signal Handler
def sig_int_callback(signum, stack):
    exit = True
    print('Handling SIGINT from thread: {}'.format(threading.current_thread().name));
    sys.exit(0)
    return

signal.signal(signal.SIGINT, sig_int_callback)

if __name__ == '__main__':
    thread.start()
    app.run(host='0.0.0.0');
