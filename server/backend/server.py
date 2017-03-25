from flask import Flask
from flask import request
import threading
import zmq
import twitter

# Construct a Flask web server object.
app = Flask(__name__)

# Construct the network ports and the thread for reading the socket.
context = zmq.Context()
socket = context.socket(zmq.PAIR)
socket.bind('tcp://0.0.0.0:5959')
thread = threading.Thread(target=check_socket)

# Default parameters for tweeting, emailing, and sending text messages.
send_email = False
send_tweet = False
send_text = False
phone_number = '123-456-7890'
email_address = 'default@gmail.com'
hashtag = 'MailsHere'

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
    arguments = requst.args()

    if 'email' in arguments and arguments['email'] is 'True':
        send_email = True;
    elif 'text' in arguments and arguments['text'] is 'True':
        send_text = True;
    elif 'tweet' in arguments:
        send_tweet = True;
    else if 'email_address' in arguments:
        # Sanitize the email address and store it.
        email_address = arguments['email_address'];
    else if 'phone_number' in arguments:
        # sanitize the phone number and store it.
        phone_number = arguments['phone_number']
    elif 'hashtag' in arguments:
        hashtag = arguments['hashtag'];

def tweet():
	return

def text():
    #TODO Try and set up text messages.
    return

def email():

def check_socket():
    msg = socket.recv()

    # TODO: Verify that the message is legit here.
    is_legit = True

    if is_legit is True:
        if send_tweet is True:
            tweet()
        if send_text is True:
            text()
        if send_email is True:
            email()


if __name__ == '__main__':
    thread.start()
    app.run();
