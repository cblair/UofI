/*  QueueL.h
 *
 *  Interface file for the ADT queue.
 *  Implementation uses a list.
 */

#ifndef QUEUE_L_H
#define QUEUE_L_H

#include "listQ.h"

typedef int QueueItemType;

class Queue
{
private:
    LinkedList  L;   //  list of queue items

public:
    Queue();
    Queue( const Queue& q );
    ~Queue();
    
    void Insert( QueueItemType newItem );
    int  Delete();
    
    int  GetFront();
    int  GetEnd();
    
    bool IsEmpty();
    
    void Print();
};
#endif
