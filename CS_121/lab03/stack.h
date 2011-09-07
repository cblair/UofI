/* stack.h
 *
 * Definition of Stack class
 */
 
#ifndef STACK_H
#define STACK_H

#include <iostream.h>
#include <assert.h>
#include "link.h"


class Stack {
public:
   Stack();
   ~Stack();

   void Push(int n);     // push item onto stack
   int Pop();            // remove item from stack
   int IsEmpty();        // is the stack empty?
   void Print();         // print the stack

private:
   LinkedList topPtr;    // pointer to list
};


#endif
