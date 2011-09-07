#include <iostream>

using namespace std;

class Node
{
public:
  int m_v;
  Node *m_next;
};

class Stack
{
private:
  Node *head;
  Node *tail;
  Node *pTemp; //pointer that will help with juggling addresses in push, etc functions.
public:
  Stack();
  void push_front(int nnVal = 0);
  void pop_front();
  void print_current(ostream &out = cout);
};

Stack::Stack()
  :head(NULL), tail(NULL)
{}

void Stack::push_front(int nnVal)
{
  //if list is empty (we need this case so we don't dereference a NULL head)
  if(head == NULL)
    {
      //TS:
      cout << "TS: 1st list condition met\n";
      head = new Node;
      head->m_v = nnVal;
      head->m_next = NULL;
    }

  //if list has one element (we need this case so we don't dereference a 
  // NULL tail)
  else if(head != NULL && head->m_next == NULL)
    {
      //TS:
      cout << "TS: 2nd list condition met\n";
      head->m_next = new Node;
      tail = head->m_next;
      tail->m_v = head->m_v;
      head->m_v = nnVal;
    }
  //2 or more elements already in the stack
  else 
    {
      pTemp = new Node;
      pTemp->m_next = head->m_next;
      pTemp->m_v = head->m_v;
      head->m_v = nnVal;
      head->m_next = pTemp;
    }
  return;
}

void Stack::pop_front()
{}

inline
ostream& operator<<(ostream &out, Stack &b)
{
  b.print_current(out);
  return(out);
}

void Stack::print_current(ostream &out)
{
  if(head != NULL) out << head->m_v;
  else if(head == NULL) out << "(empty)\n";
}

int main()
{
  Stack s1;

  cout << "The current stack value is ";
  s1.print_current(); 
  cout << '\n';

  cout << "Please enter an integer value for the first value in the stack: ";
  int temp;
  cin >> temp;
  s1.push_front(temp);
  cout << "The current stack value is ";
  s1.print_current(); 
  cout << '\n';

  cout << "Now enter the second element to be added on the stack (int value): ";
  cin >> temp;
  s1.push_front(temp);
  cout << "The current stack value is ";
  s1.print_current(); 
  cout << '\n';

  do
    {
      cout << "Enter the next value you would like to put on top of the stack"
	   << " (enter -1 when you are finished): ";
      cin >> temp;
      s1.push_front(temp);
      cout << "The new top of the list is: ";
      s1.print_current();
      cout << '\n';
    }
  while (temp != -1);
}
