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
#include <string>

using namespace std;

class BQueue
{
 private:
  struct Node
  {
    string m_name;
    bool m_dep_or_wit;
    double m_amount;
    Node *m_next;
  };
  Node *head, *pI, *pTail;
 public:
  BQueue()
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
	  while(pI != NULL)
	    {
	      ++i;
	      pI = pI->m_next;
	    }
	}
      return(i);
    }

  string front()
    {
      return(head->m_name);
    }

  string back()
    {
      if(empty()) return(0);
     
      pI = head;
      while(pI->m_next != NULL) pI = pI->m_next;
      return(pI->m_name);
    }

  void pop_back()
    {
      if(empty()) return;
      if(head->m_next == NULL) 
	{
	  pop();
	  return;
	}

      pTail = head;
      pI = pTail->m_next;
      while(pI->m_next != NULL)
	{
	  pTail = pI;
	  pI = pI->m_next;
	}
      pTail->m_next = NULL;
      delete pI;
    }

  void push(string name, bool dep_or_wit, double amount)
    {
      if(!empty()) 
	{
	  pTail = head;
	  while(pTail->m_next != NULL) pTail = pTail->m_next;
	}      
      
      pI = new Node;
      pI->m_next = NULL;
      pI->m_name = name;
      pI->m_dep_or_wit = dep_or_wit;
      pI->m_amount = amount;
      
      if(!empty()) pTail->m_next = pI;
      else head = pI;
    }

  void pop()
    {
      if(empty()) return;

      pI = head;
      head = pI->m_next;
      pI->m_next = NULL;
      delete pI;
    }
  
  void print_node(int i)
    {
      if(i > size()) return;
      if(empty()) return;
      
      pI = head;
      for(int count = 1; i != count; ++count)
	pI = pI->m_next;
      
      cout << pI->m_name << '\n';
    }
  
  //This is concering, because all info can be found for the customer
  // in the back. We can rectify this if we want by asking for passwords
  // upon entry, but at least the security risk in minimized to customers
  // in the back. It might be nicer to have a function that can move
  // any customer, but would require another argument and possibly another
  // functions, which is not necessary yet at this point.
  void get_back(string &name, bool &dep_or_wit, double &amount)
    {
      if(empty()) return;
     
      pI = head;
      while(pI->m_next != NULL) pI = pI->m_next;
        
      name = pI->m_name;
      dep_or_wit = pI->m_dep_or_wit;
      amount = pI->m_amount;
    }
};

#endif
