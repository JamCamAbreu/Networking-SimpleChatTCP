Author: James Cameron Abreu
Date: 05/05/2018
Description: Instructions on how to compile and use the 'project1' chat program
Project Source: https://github.com/JamCamAbreu/Networking-SimpleChatTCP


Files:
  - README.txt:     (This file)
  - chatserve.py    Server program, written in python
  - chatclient.c    Client program, written in C. Needs to be compiled
                    (instructions below)
  - [chatclient]    executable that is generated from C compilation instructions



Client Compilation Instructions:
  - In the directory containing the above files, execute the gcc command:
  gcc -Wall chatclient.c -o chatclient 

  - An executable called 'chatclient' will be created (execution instructions
    below)



How to use the chat program:
  1. Compile the client code (instructions above) if you haven't already.

  2. Start the server by typing "./chatserve.py 55854". Here, '55854' is an
     example port number. But you may use any port number. The server will 
     now wait for any incoming TCP connections on its socket. 

  3. In another terminal window, enter in "./client localhost 55854" 
     (Again, here 55854 is an example port number used in the step above.)
     The server and client establish a connection.

  4. The client is prompted to enter their name. (The server always has name
     'server')

  5. The client will enter a message first. The message is sent to the waiting
     server.

  6. The server will send a message next. The message is sent to the waiting
     client. 

  7. Repeat steps 5 and 6 UNTIL:

  8. Either the client or server may enter '\quit' as its message. This will
     close the connection. The server will continue to server connections until
     it receives a SIGINT (which can be done by pressing 'CONTROL+C' on the 
     server terminal at any time. Both the server and client will be aware of
     who closed the connection.



