#! /usr/bin/python
from socket import *

serverPort = 55850
serverAddress = ('localhost', serverPort)
maxQueue = 1

serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(serverAddress)
serverSocket.listen(maxQueue)

print('The server is ready to receive on %s port %s' % serverAddress)

while True:
  clientSocket, clientAddress = serverSocket.accept()
  sentence = clientSocket.recv(1024).decode()

  # debug:
  print('received: ' + sentence)

  # example:
  capitalizedSentence = sentence.upper()

  clientSocket.send(capitalizedSentence.encode())
  clientSocket.close()



print('Program closing successfully...')
