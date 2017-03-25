from flask import Flask
from flask import request
import threading
import zmq

# Construct a Flask web server object.
app = Flask(__name__)

# Construct the network ports and the thread for reading the socket.
context = zmq.Context()
socket = context.socket(zmq.REP)
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
@app.route("/", methods=['POST'])
def configure():
    arguments = requst.args()

    if 'email' in arguments and arguments['email'] is 'True':
        send_email = True;
    elif 'text' in arguments and arguments['text'] is 'True':
        send_text = True;
    elif 'tweet' in arguments:
        send_tweet = True;
    elif 'email_address' in arguments:
        email_address = arguments['email_address'];
    elif 'phone_number' in arguments:
        phone_number = arguments['phone_number']
    elif 'hashtag' in arguments:
        hashtag = arguments['hashtag'];

def tweet():
	return

if __name__ == '__main__':
    thread.start()
    app.run();
