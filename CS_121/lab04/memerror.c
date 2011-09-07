/* memerror.c
 *
 * CS 121.Bolden.........GNU Version 2...........Colby Blair
 * February 12, 2008 ....Lenovo ThinkCentre.............cblair@vandals.uidaho.edu
 *
 * This is the fixed code from Prof. Bolden's Lab 4 of CS 121
 *---------------------------------------------------------------------
 */

/* memerror.c -- try to find (and fix!) all the memory-related errors
                 in this program that

   Takes a string from the command line.  
   Makes a linked-list out of it in reverse order.  
   Traverse it to construct a string in reverse.  
   Clean up (release memory).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct st_CharNode 
{
  char theChar;
  struct st_CharNode *next;
} CharNode;


void reverseIt( char *stringbuffer );


int main( int argc, char *argv[] )
{
  char *stringBuffer;

  //  Check number of user supplied arguments.
  if( argc < 2 )
    {
      fprintf(stderr, "usage: you have to enter 1 argument.\n" );
      exit( -1 );
    }

  if( argc != 2 )
    {
      fprintf( stderr, "usage: %s string.  This reverses the string "
	       "given on the command line\n", argv[1] );
      exit( -1 );
    }
  
  // Copy the argument so we can make changes to it
  //stringBuffer = (char*) malloc( strlen(argv[1]) );
  //strcpy( argv[1], *stringBuffer );
  stringBuffer = argv[1];

  // Reverse the string
  reverseIt( stringBuffer );

  // Print the reversed string
  printf( "the reversed string is '%s'\n", stringBuffer );

  return 0;
}


// Build a linked list backwards, then traverse it.

void reverseIt( char *stringbuffer )
{
  CharNode *head, *node;
  char *scan, *stop;
    
  // initialize local vars
  head = node = NULL;

  // find the start and end of the string so we can walk it
  scan = stringbuffer;
  //TS - stop = stringbuffer + strlen(stringbuffer) + 1;
  stop = stringbuffer + strlen(stringbuffer);

  // walk the string
  while (scan < stop)
    {
      if (head == NULL)
        {
	  head = (CharNode*) malloc( sizeof(CharNode*) );
	  head->theChar = *scan;
	  head->next = NULL;
        }
      else
        {
	  node = (CharNode*) malloc( sizeof(CharNode*) );
	  node->theChar = *scan;
	  node->next = head;
	  head = node;
        }
      scan++;
    }

  // Re-point to the buffer so we can drop the characters
  scan = stringbuffer;

  //  Traverse the nodes and add them to the string
  while( head != NULL )
    {
      *scan = head->theChar;
      free( head );
      node = head->next;
      head = node;
      scan++;
    }

  // Release head
  free( head );   
}
