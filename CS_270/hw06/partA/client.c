/* server.c
 *
 * CS 270.Rinker.........GNU Version 2...........Colby Blair
 * 11/05/09 ...........Dell Inspiron 1521........cblair@vandals.uidaho.edu
 * Assignment 05

 *******************Client Main********************
This file in the main execution of client
*/

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include "cbstrings.h"

#define MAX_BUFFER_SIZE 1024

char buffer[MAX_BUFFER_SIZE];

void set_buffer_empty()
{
  int i;
  for(i = 0; buffer[i] != '\0'; i++)
    {
      buffer[i] = '\0';
    }
}

int main()
{
  int sckt, portnum, status;
  struct sockaddr_in servadd;
  struct hostent *server;

  while(1)
    {
      //open socket
      portnum = 4502;
      sckt = socket(AF_INET, SOCK_STREAM, 0);
      if(sckt < 0)
	printf("ERROR: Could not open socket\n");
      server = gethostbyname("127.0.0.1");
      if(server == NULL)
	printf("ERROR: Host was not available\n");
            
      //connect
      servadd.sin_family = AF_INET;
      servadd.sin_port = htons(portnum);
      bcopy((char *)server->h_addr, 
	    (char *)&servadd.sin_addr.s_addr,
	    server->h_length);
      if (connect(sckt, &servadd, sizeof(servadd)) < 0) 
	printf("ERROR connecting\n");
      
      //user input command
      printf("Connected to server, enter command: ");
      fgets(buffer,255,stdin);
      status = send(sckt, buffer, strlen(buffer), 0);
      if(status < 0)
	printf("ERROR: Could not send message\n");
      
      //recieve confirmation from server
      status = recv(sckt, buffer, MAX_BUFFER_SIZE, 0);
      if(status < 0)
	printf("ERROR: Could not reciever status from server\n");
      else
	printf("Server Response: %s\n", buffer);
      if(str_starts_with(buffer, "quit"))
        {
          exit(0);
        }

      set_buffer_empty();
    }

  return(0);
}
