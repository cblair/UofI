/* TestHash.cpp
 *   Test the Hash table code.
 */

#include <iostream>
//#include "stdlib.h"
using namespace std;

#include "hash.h"
#include "hash.cpp"

int main()
{  
      //  Put a few values in the table...
   (void)Insert( "One",  "1" );
   (void)Insert( "One",  "11" );
   (void)Insert( "Two",  "2" );
   (void)Insert( "Four", "4" );
   (void)Insert( "Five", "5" );
   (void)Insert( "Six",  "6" );
   (void)Insert( "Nine", "9" );
   
   (void)Insert( "Yes", "1" );
   (void)Insert( "YES", "1" );
   (void)Insert( "No",  "0" );
   (void)Insert( "NO",  "0" );
   
   PrintHashTable();
         
   return EXIT_SUCCESS;
}
