/*  queueL.cpp
 *
 *  Definition file for the ADT queue.
 *  Implementation uses a list.
 */

#include "queueL.h"


Queue::Queue()
{
    //  default constructor
}

Queue::Queue( const Queue& q ) : L(q.L)
{
    //  copy constructor
}

Queue::~Queue()
{
    //  destructor
}
//  \end{verbatim}  \lecpb  \begin{verbatim}
void Queue::Insert( QueueItemType newItem )
{
    L.AddNode( newItem );
}

int Queue::Delete()
{
    int iVal;

    iVal = L.FirstNode();
    L.DeleteNode();
    
    return iVal;
}


int Queue::GetFront()
{
    return  L.FirstNode();
}

int Queue::GetEnd()
{
    return  L.LastNode();
}

bool Queue::IsEmpty()
{
    int  length = L.Size();
    return bool(length == 0 );
}

void Queue::Print()
{
    L.PrintNodes();
}


