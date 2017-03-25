from flask import Flask
from flask import request
import threading
import zmq
import twitter

# Construct a Flask web server object.
app = Flask(__name__)

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
    arguments = dict(request.form)
    print('Received an update request: {}'.format(arguments))

    if 'email' in arguments and arguments['email'] is 'True':
        send_email = True;
    elif 'text' in arguments and arguments['text'] is 'True':
        send_text = True;
    elif 'tweet' in arguments:
        send_tweet = True;
    elif 'email_address' in arguments:
        # Sanitize the email address and store it.
        email_address = arguments['email_address'];
    elif 'phone_number' in arguments:
        # sanitize the phone number and store it.
        phone_number = arguments['phone_number']
    elif 'hashtag' in arguments:
        hashtag = arguments['hashtag'];
    return ''

def tweet():
    return

def text():
    #TODO Try and set up text messages.
    return

def email():
    return

def check_socket():
    while True:
        msg = socket.recv()
        print ('Packet: {}'.format(msg))

        # TODO: Verify that the message is legit here.
        is_legit = True

        if is_legit is True:
            if send_tweet is True:
                tweet()
            if send_text is True:
                text()
            if send_email is True:
                email()

# Construct the worker thread for the receiving function
thread = threading.Thread(target=check_socket)
thread.daemon = True
if __name__ == '__main__':
    thread.start()
    app.run(host='0.0.0.0');
