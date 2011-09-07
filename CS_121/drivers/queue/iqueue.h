/*
Author: Colby Blair
This file is a Queue class with the following structure:
Node: 1 int var
members:
         empty
	 size
	 front
	 back
	 push
	 pop
*/

#ifndef _iqueue_h_included
#define _iqueue_h_included

#include <iostream>

using namespace std;

class IQueue
{
 private:
  struct Node
  {
    int m_val;
    Node *m_next;
  };
  Node *head, *pI, *pTail;
 public:
  IQueue()
    : head(NULL), pI(NULL), pTail(NULL)
    {}
  
  bool empty()
    {
      return( (head == NULL) );
    }

  int size()
    {
      int i = 0;
      if(!empty())
	{
	  pI = head;
	  while(pI->m_next != NULL)
	    {
	      ++i;
	      pI = pI->m_next;
	    }
	}
      return(i);
    }

  int front()
    {
      return(head->m_val);
    }

  int back()
    {
      if(empty()) return(0);
     
      pI = head;
      while(pI->m_next != NULL) pI = pI->m_next;
      return(pI->m_val);
    }

  void push_front(int i)
    {
      if(!empty()) 
	{
	  pTail = head;
	  while(pTail->m_next != NULL) pTail = pTail->m_next;
	}      
      
      pI = new Node;
      pI->m_next = NULL;
      pI->m_val = i;
      
      if(!empty()) pTail->m_next = pI;
      else head = pI;
    }

  void pop_back()
  {
    if(empty()) return;
    if(size() == 1)
      {
	head->m_next = NULL;
	delete head;
	head = NULL;
	return;
      }
    
    //if evaluation gets to this point, the queue
    // has 2 or more items in it.
    pTail = head;
    do
      {
	pTail = pTail->m_next;
	pI = pTail->m_next;
      }
    while(pI->m_next != NULL);

    pTail->m_next = NULL;
    pI->m_next = NULL;
    delete pI;
  }
};
#endif
