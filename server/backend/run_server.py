from server import Server

from flask import Flask
from flask import request
import sys
import threading
import signal
import time

# Construct a Flask web server object.
app = Flask(__name__)
server = Server()

@app.route('/home')
def default_page():
    return 'Operation Expert Guacamole is a go.'

# Configures the server to send texts, tweet, or send emails.
#
# Available names:
#   email={True|False}
#   phone={True|False}
#   tweet={True|False}
#   email_address={address}
#   phone_number={number}
#   hashtag={tag}
@app.route("/update", methods=['POST'])
def configure():

    arguments = dict(request.form)
    print('Received an update request: {}'.format(arguments))

    email_address = None
    phone_number = None
    hashtag = None
    if 'email' in arguments and 'email_address' in arguments and arguments['email'][0].encode('ascii', 'ignore') == 'True':
        email_address = arguments['email_address'][0].encode('ascii', 'ignore')
    if 'phone' in arguments and 'phone_number' in arguments and arguments['phone'][0].encode('ascii', 'ignore') == 'True':
        phone_number = arguments['phone_number'][0].encode('ascii', 'ignore')
    if 'tweet' in arguments and 'hashtag' in arguments:
        hashtag = arguments['hashtag'][0].encode('ascii', 'ignore')

    server.update(phone_number, email_address, hashtag)

    return ''

# CTRL+C Signal Handler
def sig_int_callback(signum, stack):
    server.kill_thread()
    print('Handling SIGINT from thread: {}'.format(threading.current_thread().name));
    sys.exit(0)
    return

signal.signal(signal.SIGINT, sig_int_callback)

if __name__ == '__main__':
    app.run(host='0.0.0.0');
