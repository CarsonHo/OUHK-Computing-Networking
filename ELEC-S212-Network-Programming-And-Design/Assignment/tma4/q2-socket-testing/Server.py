import hashlib
import socket
import sys
import threading
import argparse
import re
import uuid
from collections import OrderedDict

def prepareString(content):
	return bytes(content + "\n", 'UTF-8')

def getArguments(message):
	return message.split(" ")[1:]

def helloRequest(thread):
	thread.send("Message Hello from " + socket.gethostname())

def loginRequest(thread):
	user = getArguments(thread.data['message'])[0]
	if user in Database.user.keys():
		thread.data['login'] = user
		thread.data['token'] = str(uuid.uuid4())
		thread.send("CHALLENGE " + thread.data['token'])
	else:
		thread.send("MSG Indicated user not found")
	
def challengeRequest(thread):
	passwordMessage = thread.data['token'] \
	                + Database.user[thread.data['login']]
	hashObject = hashlib.sha512(passwordMessage.encode('UTF-8'))
	hexDigest = hashObject.hexdigest() 
	
	# Get hash from ∏received message
	hashRequest = getArguments(thread.data['message'])[0]
	
	# Test connection
	if hexDigest == hashRequest:
		thread.data['isLogged'] = True
		thread.send("MSG Login success.")
	else:
		thread.send("MSG Login failure.")
	
	# Reset token
	thread.data['token'] = ""
	
def isLoggedRequest(thread):
	if 'isLogged' in thread.data.keys():
		thread.send("MSG You are logged in.")
	else:
		thread.send("MSG You are not logged in.")

def logoutRequest(thread):
	if 'isLogged' in thread.data.keys():
		del thread.data['isLogged']
		thread.send("MSG You are now logged out.")
	else:
		thread.send("MSG You are not logged in.")
	
def closeConnectionRequest(thread):
	thread.closeConnection = True
	thread.send("CLOSE CONFIRM")

def notFoundRequest(thread):
	thread.send("MSG Command not found")

#Class

class Database:
	"""Contains data used in application"""
	user = {}
	
	@staticmethod
	def load(databaseFile):
		# Open database file
		with open(databaseFile) as file:
			# Read each line (format "user:password")
			for line in file:
				# Delete new line character and split
				data = line.rstrip('\n').split(':')
				
				if len(data) == 2:
					Database.user[data[0]] = data[1]

class ThreadClient(threading.Thread):
	"""Manage each client connection to the server in a new thread"""
	
	functionArray = OrderedDict([
		(r"HELLO", helloRequest),
		(r"LOGIN .*", loginRequest), 
		(r"CHALLENGE .*", challengeRequest), 
		(r"IS_LOGGED", isLoggedRequest),
		(r"LOGOUT", logoutRequest),
		(r"CLOSE", closeConnectionRequest),
		(r".*", notFoundRequest),
	])
	
	def __init__(self, connection):
		threading.Thread.__init__(self)
		self.connection = connection
		self.closeConnection = False
		
		self.data = {}
		self.data['message'] = ""
		
		print("%s connected" % self.getName())
	
	def send(self, message):
		print("%s -> %s" % (self.getName(), message))
		self.connection.send(prepareString(message))

	def run(self):
		try:
			while not self.closeConnection:
				
				character = self.connection.recv(1)
				
				if character == b'\n':
				
					print("%s <- %s" % (self.getName(), self.data['message']))
					
					for regex, function in ThreadClient.functionArray.items():
						if re.match(regex, self.data['message']):
							function(self)
							break
					
					self.data['message'] = ""
				else:
					self.data['message'] += character.decode('UTF-8')
				
		except ConnectionResetError:
			print("%s has reset connection" % self.getName())
		
		# Close connection
		self.connection.close()
		del connectionList[self.getName()]
		print("%s disconnected" % self.getName())

#Main

# If this is the main module, run this
if __name__ == '__main__':
	argsCount = len(sys.argv)

	# Create argument parser to help user
	parser = argparse.ArgumentParser(
		description='Server instance for challenge response application.'
	)
	parser.add_argument(
		'database', 
		type=str,
		help='Database of users and password to use.'
	)
	parser.add_argument(
		'port', 
		nargs='+',
		type=str,
		help='Port to use to receive connections (Default: 1991).'
	)

	# Show help if one of the arguments is missing
	if argsCount not in [2, 3]:
		parser.print_help()
		sys.exit()
	
	# Get configuration
	host = '127.0.0.1'
	database = sys.argv[1]
	port = int(sys.argv[2]) if argsCount == 3 else 1991
	
	# Load user database
	try:
		Database.load(database)
	except FileNotFoundError:
		print("Database user not found")
		sys.exit(0)
	
	# Server initialization
	serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	try:
		serverSocket.bind((host, port))
	except socket.error:
		print("Socket creation failed, port is using?")
		sys.exit()
	
	# Listen new connection
	print("Server ready, waiting for requests ...")
	serverSocket.listen(1)

	# Connection list (clients)
	connectionList = "RECV"
	
	try:
		while True:    
			connection, adresse = serverSocket.accept()
			
			# Start a new client
			thread = ThreadClient(connection)
			thread.start()
			
			
			# Store connection
			id = thread.getName()
			connectionList = connection
			
			# Send a welcome message
			thread.send("OU-S212-S1223662-HOHOIYIN!")
	
	except KeyboardInterrupt:
		print("Server shut down")

