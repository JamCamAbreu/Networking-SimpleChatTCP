#! /usr/bin/python
from socket import *
import signal
import sys

serverPort = 55851
serverAddress = ('localhost', serverPort)
maxQueue = 1

exitCode = '\\quit'
serverName = 'chatServer'

serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(serverAddress)
serverSocket.listen(maxQueue)


def sigint_handler(signum, frame):
  serverSocket.close()
  print " Connection Closed."
  sys.exit(0)

signal.signal(signal.SIGINT, sigint_handler)


while True:

  exitStatus = False
  print('The server is ready to receive on %s port %s' % serverAddress)

  clientSocket, clientAddress = serverSocket.accept()
  print('Client from %s port %s connected.' % clientAddress)

  while (exitStatus is False):

    print('Client writing message...')
    sentence = clientSocket.recv(1024).decode()

    if exitCode in sentence:
      print('Client closed connection')
      exitStatus = True

    else:
      # print received message:
      print(sentence)

      raw = raw_input('Enter message to send: ')
      m = serverName + '> ' + raw[:500]

      if exitCode in m:
        m = '\\quit'
        clientSocket.send(m.encode())
        exitStatus = True

      else:
        clientSocket.send(m.encode())

  clientSocket.close()




clientSocket.close()
print('Receiving Socket closed. Ending Server.')
