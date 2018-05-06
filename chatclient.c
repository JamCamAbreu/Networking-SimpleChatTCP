#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

const int MAX_CHARS = 500;

// Error function used for reporting issues
void error(const char *msg) { perror(msg); exit(0); } 

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *         Get Message (Input)           
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void getMessage(char* buffer, int bufferSize, char* name, int nameSize) {
  memset(buffer, '\0', bufferSize); // Clear out the buffer array
  memcpy(buffer, name, nameSize); // add name to buffer first

  char* Loc = strchr(buffer, '\0');
  int curLoc = (int)(Loc - buffer);

  buffer[curLoc] = '>'; // add arrow
  buffer[++curLoc] = ' '; // add space after name

  char inputBuff[512];
  memset(inputBuff, '\0', 512); // Clear out the buffer array

  // get message from user:
  fgets(inputBuff, MAX_CHARS + 1, stdin); // plus 1 for \0

  // discard any extra characters from stdin
  if (!strchr(inputBuff, '\n'))   // new line does not exist
    while (fgetc(stdin) != '\n'); // discard until newline

  inputBuff[strcspn(inputBuff, "\n")] = '\0'; // Remove the trailing \n that fgets adds

  // copy message over to final buffer:
  memcpy(&buffer[++curLoc], inputBuff, sizeof(inputBuff));
}



  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *        Send Message To User           
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void sendMessage(int socket, char* buffer) {
  int charsWritten = send(socket, buffer, strlen(buffer), 0); // Write to the server
  if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
  if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");
}



  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *        Set up user name               
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void initName(char* buffer, int bufferSize) {
  printf("Please enter a desired username with max 10 characters: ");
  memset(buffer, '\0', bufferSize); // Clear out the buffer array
  fgets(buffer, bufferSize - 1, stdin); // Get input from the user, trunc to buffer - 1 chars, leaving \0

  // discard any extra characters:
  if (!strchr(buffer, '\n'))
    while (fgetc(stdin) != '\n');

  buffer[strcspn(buffer, "\n")] = '\0'; // Remove the trailing \n that fgets adds
}


  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *       Check for Exit command          
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int checkExit(char* command, char* string) {
  if (strstr(string, command) != NULL)
    return 1; // command was found
  else
    return 0; // command was NOT found
}




  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *        MAIN PROGRAM                   
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int main(int argc, char *argv[]) {
  int socketFD, portNumber, charsRead;
  int exitStatus = 0; // false until proven true
  struct sockaddr_in serverAddress;
  struct hostent* serverHostInfo;
  char buffer[1024];
  int maxLenName = 10;
  char username[maxLenName + 1];
  char exitCommand[] = "\\quit";
    
  // Check usage & args
  if (argc < 3) { fprintf(stderr,"USAGE: %s [hostname] [port]\n", argv[0]); exit(0); } 

  // Set up the server address struct
  memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
  portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
  serverAddress.sin_family = AF_INET; // Create a network-capable socket
  serverAddress.sin_port = htons(portNumber); // Store the port number
  serverHostInfo = gethostbyname(argv[1]); // Convert the machine name into a special form of address
  if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
  // Copy in the address
  memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); 

  // Set up the socket
  socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
  if (socketFD < 0) error("CLIENT: ERROR opening socket");
  
  // Connect to server (Connect socket to address)
  if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) 
    error("CLIENT: ERROR connecting");


  // input username
  initName(username, sizeof(username));


  /*  +-----------------------------------+
      |     Messaging Starts/Flows Here   |
      +-----------------------------------+ */
  while (1) {
    // Get input message from user
    printf("Enter message to send: ");
    getMessage(buffer, sizeof(buffer), username, sizeof(username));

    // Check if user entered the quit command:
    exitStatus = checkExit(exitCommand, buffer);

    // User DID enter quit command. Send quit message to server and close socket:
    if (exitStatus == 1) {
      printf("Closing socket and exiting program.\n");
      sendMessage(socketFD, buffer); // let server know we are quitting
      close(socketFD); // Close the socket
      exit(0);
    }

    // User did NOT enter quit command. Send message and continue:
    else {

      // Send message to server
      sendMessage(socketFD, buffer);

      // Get return message from server
      printf("Server writing message...\n");
      memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
      charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
      if (charsRead < 0) error("CLIENT: ERROR reading from socket");

      // server closed connection?
      if (checkExit(exitCommand, buffer)) {
        printf("The server closed the connection. Exiting.\n");
        close(socketFD); // Close the socket
        exit(0);
      }

      // print server message:
      printf("%s\n", buffer);
    } // end else


  } // end while true
  return 0;
}


