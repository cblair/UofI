#include "iqueue.h"
#include <iostream>

using namespace std;

int main()
{
  IQueue q1;

  if(q1.empty()) cout << "(empty)\n";
  else cout << "not empty\n";
  for(int i = 0; i < 10; ++i)
    q1.push_front(i);
  if(q1.empty()) cout << "(empty)\n";
  else cout << "not empty\n";
  cout << "Front = " << q1.front() << '\n';
  cout << "Back = " << q1.back() << '\n';
  //print();
  cout << "size = " << q1.size() << '\n';
  while(q1.size() > 0)
    {
      q1.pop_back();
      cout << "size = " << q1.size() << '\n';
      //print();
    }
}
