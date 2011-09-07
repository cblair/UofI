/*  hash.h
 */

#ifndef HASH_H
#define HASH_H

#include "buckets.h"

struct nList     /*  table entry:  */
{
   char *name;          /*  defined name         */
   char *defn;          /*  replacement text     */
   struct nList *next;  /*  next entry in chain  */
};

typedef struct nList *NListPtr;

unsigned Hash( char *s );
NListPtr Lookup( char *s );
NListPtr Insert( char *name, char *defn );

void PrintHashTable();

#endif  
/*  HASH_H  */
