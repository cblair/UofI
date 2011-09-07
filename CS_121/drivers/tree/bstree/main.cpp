#include <iostream>

using namespace std;

#include "bstree.h"

//Stolen from http://www.cplusplus.com/reference/clibrary/ctime/clock.html
void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

int main()
{
    BSTree tree1;
          
    cout << "Test tree program. Enter the amount of elements in the tree: ";
    int itemp;
    cin >> itemp;
    for(int i = 1; i <= itemp; ++i)
            tree1.InsertNode(i);  
    cout << "Printing tree1:\n";
    tree1.PrintTree();
    
    int i = 1;
    while(i < 10)
    {
            cout << i << ", ";
            wait(1); 
            ++i;
    }
    cout << i << '\n';
}
