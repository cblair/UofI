/* Hash.cpp
 *
 *  Hash table implementation from:
 *  Kernighan & Ritchie, The C Programming Language,
 *     Second Edition, Prentice-Hall, 1988.
 */

#include <iostream>
#include <iomanip>
//#include <stdlib>
using namespace std;
#include <string>

#include "hash.h"

const int HASH_TABLE_SIZE = 101;
static NListPtr hashTable[HASH_TABLE_SIZE];

    //  Prototypes
char *strdup( const char * );  //  in string.h, but....



/*  Hash
 *  Generate hash value for string s
 */

unsigned Hash( char *s )
{
    unsigned hashVal;
    
    for( hashVal = 0 ; *s != '\0' ; s++ )
        hashVal = *s + 31 * hashVal;
        
    return  hashVal % HASH_TABLE_SIZE;
}


/*  Lookup
 *  Look for s in hashTable
 */

NListPtr Lookup( char *s )
{
    NListPtr np;
    
    for( np = hashTable[Hash(s)] ; np != NULL ; np = np->next )
      {
        if( strcmp(s, np->name) == 0 )
	  return np;    //  found
      }
    
    return NULL;          //  not found
}

/*  Insert
 *  Put (name, defn) in hash table
 */
 
NListPtr Insert( char *name, char *defn )
{
    unsigned hashVal;
    NListPtr np;
    
    if( (np = Lookup(name)) == NULL )  // not found
    {
        np = (NListPtr) malloc(sizeof(*np));
        if( np == NULL || (np->name = strdup(name)) == NULL )
            return NULL;
        hashVal = Hash(name);
        np->next = hashTable[hashVal];
        hashTable[hashVal] = np;
    }
    else
    {      //  remove previous definition
        free( (void *)np->defn );
    }
    
    if( (np->defn = strdup(defn)) == NULL )
        return NULL;
    
    return np;
}


/*  PrintHashTable
 *  Print the hash table contents
 */

void PrintHashTable()
{
    NListPtr np;
    Buckets bucketInfo1;

    cout << "Hash table contents:" << endl;
    cout << "--------------------\n" << endl;

    //4.
    bucketInfo1.bucketSize = 0;
    for( int i = 0 ; i < HASH_TABLE_SIZE ; i++ )
    {
      bucketInfo1.bucketSize = 0;
      np = hashTable[i];
      while( np != NULL )
        {
	  ++bucketInfo1.bucketSize;
	  cout << setw(3) << i << ":    ";
	  cout << np->name << ", " << np->defn;
	  cout << endl;
	  np = np->next;
        }
      ++bucketInfo1.bucketQty;
      cout << "Bucket size = " << bucketInfo1.bucketSize
	   << ", total bucket quantity is " << bucketInfo1.bucketQty
	   << ".\n"
	;
      //5.
      bucketInfo1.maxAndMin();
    }
    cout << "The minimum bucket size is " << bucketInfo1.bucketMin 
	 << ", and the maximum bucket size is " << bucketInfo1.bucketMax
	 << ".\n"
      ; 
}


/*  strdup
 *  Make a duplicate copy of s
 */

char *strdup( const char *s )
{
    char *p;
    
    p = (char *) malloc(strlen(s)+1);  /*  +1 for '\0'  */
    if( p != NULL )
        strcpy(p,s);

    return p;
}
