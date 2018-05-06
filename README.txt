Author: James Cameron Abreu
Date: 05/05/2018
Description: Instructions on how to compile and use the 'project1' chat program
Project Source: https://github.com/JamCamAbreu/Networking-SimpleChatTCP


Files:
  - README.txt:     This file
  - server.py       Server program, written in python
  - client.c        Client program, written in C. Needs to be compiled
                    (instructions below)
  - [client]        executable that is generated from C compilation instructions



Client Compilation Instructions:
  - In the directory containing the above files, execute the gcc command:
  gcc -Wall client.c -o client

  - An executable called 'client' will be created (execution instructions
    below)



How to use the chat program:
  1. Compile the client code (instructions above) if you haven't already.

  2. Start the server by typing "./server.py". The server will wait for any
     incoming TCP connections on its socket. 

  3. In another terminal window, enter in "./client localhost 55851" The 
     server and client establish a connection.

    - Note: if that port is already used on your system, you either have to
      change the port used in the server.py file using vim, or operate on
      another machine. 

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



