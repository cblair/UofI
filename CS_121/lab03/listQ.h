/*  listQ.h
 *
 *  This is a class for a linked list of integers
 *  used in queues.
 *  MOD: Comment to test makefile
 */

#include <bool.h>
#include <iostream.h>


class LinkedList
{
private:

  struct node
  {
     int info;
     node * next;
  };

  typedef node * nodeptr;

  nodeptr start;    //  pointer to front
  nodeptr end;      //  pointer to end

  int count;

public:

       // Constructor
   LinkedList()
   {
      start = NULL;
      end   = start;
      count = 0;
   }

       // Destructor
   ~LinkedList()
   {
      nodeptr p = start, n;
 
      while (p != NULL)
      {
         n = p;
         p = p->next;
         delete n;
      }
   }
//  \end{verbatim}  \lecpb  \begin{verbatim} 
    // Put a node at the end of the linked list.

   void AddNode(int x);

    //  Delete the first node in the list.

   void DeleteNode();

    //  Return the first or last node.
    
   int  FirstNode();
   int  LastNode();
   
    //  Output the values in the nodes, one integer per line.

   void PrintNodes();

    //  Return true if a node with the value x
    //  is in the list.

   bool IsInList(int x);

    //  Return a count of the number of nodes in the list.
 
   int Size();
};

