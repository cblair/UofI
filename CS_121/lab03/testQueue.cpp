/*  testQueue.cpp
 */

#include <iostream.h>

#include "queueL.h"


int main()
{
    Queue q1 ;
    int qVal;


      //  add some initial nodes
    q1.Insert( 3 );
    q1.Insert( 5 );

    cout << "Initial contents of q1:" << endl;
    q1.Print();

      //  add a few more nodes
    q1.Insert( 1 );

    cout << "Contents of q1 after adding:" << endl;
    q1.Print();

      //  delete a few items
    cout << "\nDeleting two items from queue:" << endl;
    
    qVal = q1.Delete();
    cout << "Contents of q1 after deleting one item:" << endl;
    q1.Print();
    
    qVal = q1.Delete();
    cout << "Contents of q1 after deleting one item:" << endl;
    q1.Print();
}
