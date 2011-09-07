#ifndef _hash_h
#define _hash_h

#include <string>
#include <iomanip>

#include "buckets.h"

struct Node
{
  string m_state;
  string m_capital;
  int m_population;
  Node *m_next;
};

static const int HASH_TABLE_SIZE = 91;
Node *hash_table[HASH_TABLE_SIZE];

void init()
{
  for(int i = 0; i < HASH_TABLE_SIZE; ++i)
    hash_table[i] = NULL;
}

int hash(string &state)
{
  int checksum = 0;
  for(int i = 0; state[i] != '\0'; ++i)
    checksum += static_cast<int>(state[i]);
  return(checksum % HASH_TABLE_SIZE);
}

void insert(string &state, string capital, int population)
{
  int i = hash(state);
  Node *pI = hash_table[i];

  Node *pNew = new Node;
  pNew->m_next = NULL;
  pNew->m_state = state;
  pNew->m_capital = capital;
  pNew->m_population = population;

  while(pI != NULL && pI->m_next != NULL) pI = pI->m_next;
  if(pI == NULL) hash_table[i] = pNew;
  else pI->m_next = pNew;
}

void print_hash_table()
{
  Node *pI = NULL;
  Buckets bucketInfo1;

  for(int i = 0; i < HASH_TABLE_SIZE; ++i)
    {
      pI = hash_table[i];
      bucketInfo1.bucketSize = 0;
      while(pI != NULL)
	{
	  cout << i << setw(9) << ": "
	       << setw(20) << pI->m_state
	       << setw(20) << pI->m_capital
	       << setw(20) << pI->m_population
	       << '\n'
	    ;
	  pI = pI->m_next;
	  ++bucketInfo1.bucketSize;
	}
      cout << "Bucket size for this element is " 
	   << bucketInfo1.bucketSize << '\n'
	;
    }
}

#endif
