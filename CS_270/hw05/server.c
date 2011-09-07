/* server.c                                                   
 *                                                                              
 * CS 270.Rinker.........GNU Version 2...........Colby Blair      
 * 11/05/09 ...........Dell Inspiron 1521........cblair@vandals.uidaho.edu  
 * Assignment 05

 *******************Server Main********************                          
This file in the main execution of server
*/

#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "cbstrings.h"
#include <time.h>

#define MAX_BUFFER_SIZE 5000
char buffer [MAX_BUFFER_SIZE];

char *interprut_request(char *request)
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );

  if(str_starts_with(request, "timedate"))
    {
      strftime (buffer, MAX_BUFFER_SIZE, "%c", timeinfo);
      return(buffer);
    }
  if(str_starts_with(request, "time"))
    {
      strftime (buffer, MAX_BUFFER_SIZE, "%X", timeinfo);
      return(buffer);
    }
  if(str_starts_with(request, "date"))
    {
      strftime (buffer, MAX_BUFFER_SIZE, "%A %B %d %Y", timeinfo);
      return(buffer);
    }
  if(str_starts_with(request, "quit"))
    {
      return("quit");
    }

  //else
  return("Sorry, request was not understood\n");
}

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
  int sckt, portnum, newsckt, clisize, status;
  struct sockaddr_in servadd, cliadd;

  //open socket 
  sckt = socket(AF_INET, SOCK_STREAM, 0);
  
  if(sckt < 0)
    {
      printf("ERROR: Could not open socket\n");
    }
  
  //bind the port
  portnum = 4502;
  servadd.sin_family = AF_INET;
  servadd.sin_addr.s_addr = INADDR_ANY;
  servadd.sin_port = htons(portnum);
  if(bind(sckt, (struct sockaddr *) &servadd, sizeof(servadd)) < 0)
    printf("ERROR: Could not bind!\n");


  while(1)
    { 
      //listen   
      listen(sckt, 5);
      
      //accept connection on socket 
      clisize = sizeof(cliadd);
      printf("Waiting for client connection...\n");
      newsckt = accept(sckt, (struct sockaddr *) &cliadd, &clisize);
      if(newsckt < 0)
	printf("ERROR: Could not accept!\n");
      
      //read message from client
      status = recv(newsckt, buffer, MAX_BUFFER_SIZE, 0);
      if(status < 0)
	printf("ERROR: Could not read from socket\n");
      printf("Client send message: %s\n", buffer);
      
      char *message;
      message = interprut_request(buffer);
      //message = "Message recieved";
      printf("Sending response: %s\n", message);
      status = send(newsckt, message, strlen(message), 0);
      if(str_starts_with(message, "quit"))
	{
	  exit(0);
	}
      if(status < 0)
	printf("ERROR: Could not write to socket\n");
      
      set_buffer_empty();
    }

  return(0);

}

