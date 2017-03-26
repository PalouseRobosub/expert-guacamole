# expert-guacamole

# Server Backend requirements
Install python Flask and pyzmq packages:
    sudo pip install flask pyzmq werkzeug jinja2 itsdangerous click twython twilio

Install ZMQ libraries:
    sudo aptitude install libzmq3-dev

# Server Frontend Setup
Install NodeJS 6
	- curl -sL http://deb.nodesource.com/setup_6.x | sudo -E bash -
	- sudo apt install nodejs
	- sudo npm install -g nodemon
Install Dependencies
	- cd server/frontend
	- npm install
Start Server
	- nodemon
