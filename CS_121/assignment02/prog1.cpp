/* 01/26/08 Note:
Implementation time: 14:20 - 15:40, 1 hour 20 min
Implementation time: 16:20 - 17:40  1 hour 20 min
Implementation time: 19:40 - 21:40  2 hours
*/

#include <iostream>
#include <string>

using namespace std;

class HotPotato
{
private:
  int init_N; //the initial amount of people
  int init_M; //the amount of passes that will terminate a player
  struct Node
  {
    int m_ID; //the id that numbers the player.
    Node *m_next;
  };
  Node *head;
  Node *pI;
  Node *pTail;
public:
  int curr_N; //The current amount of people in the circle
  int curr_M; //The current count on the potato around the circle
  int curr_holder; //The identifier of the person who holds the potato
  HotPotato()
    : init_N(0), init_M(0),
      head(NULL), pI(NULL), pTail(NULL),
      curr_N(0), curr_M(0), curr_holder(1)
  {}

  //The initial function to input values to N and M
  bool input()
  {
    cin >> init_N >> init_M;
    if(init_N < 1) return(false);

    return(true);
  }

  //If the list is empty,Builds the list 
  //of N people, and return true
  bool build_game()
  {
    if(head != NULL)
      return(false);

    for(int i = 1; i <= init_N; ++i)
      {
	pI = new Node;
	pI->m_next = NULL;
	pI->m_ID = i;
	if(head == NULL) head = pI;
    	else
	  {
	    pTail = head;
	    while(pTail->m_next != NULL) pTail = pTail->m_next;
	    pTail->m_next = pI;
	  }
      }
    curr_N = init_N;
    curr_M = init_M;
    return(true);
  }

  //Prints the full list of people existing.
  void print_list()
  {
    for(pTail = head; pTail != NULL; pTail = pTail->m_next)
      cout << pTail->m_ID << '\n';
  }

  //Passes the potato to the next holder, unless it is the
  // last holder on the list; otherwise, pass it to the 
  // first of the this.
  int pass_potato()
  {
    if(curr_holder < curr_N) ++curr_holder;
    else curr_holder = 1;
    --curr_M;
  
    pTail = head;
    for(int i = 1; i <= curr_holder - 1; ++i)
      {
	if(pTail->m_next == NULL)
	  pTail = head;
	else pTail = pTail->m_next;
      }
    return(pTail->m_ID);
  }

  //If there is more than 1 person in the list, find
  // the current holder in the list, and delete from
  // the list.
  int kill_curr_holder()
  {
    if(curr_N <= 1) return(curr_holder);
    
    pTail = head;
    for(int i = 1; i < curr_holder - 1; ++i)
      {
      if(pTail->m_next == NULL)
	pTail = head;
      else pTail = pTail->m_next;
      }
    
    if(pTail->m_next == NULL) 
      {
	pI = head;
	head = head->m_next;
      }
    else 
      {
	pI = pTail->m_next;
	pTail->m_next = pI->m_next;
      }
    
    cout << "TS: Player being killed: " << pI->m_ID << '\n';
    pI->m_next = NULL;
    delete pI;
    --curr_N;

    return(head->m_ID);
  }

  //Reorganize circle after a kill
  void reset_circ()
  { 
    curr_M = init_M;
  }

  //Returns the int ID of the winner.
  int winner()
  {
    if(curr_N == 1) return(head->m_ID);
    return(-1);
  }
};

int main()
{
  HotPotato game1;

  cout << "Enter in the amount of people in the circle (\'N\'),\n"
       << "and the amount of passes on the potato for elimination "
       << "(\'M\'): "
    ;
  while(!game1.input())
    {
      cout << "Error: The N amount of people must be greater "
	   << "than 0. Please try again, or press Ctrl ^ C "
	   << "to exit: "
	;
    }
  game1.build_game();
  game1.print_list();

  while(game1.curr_N > 1)
    {
      if(game1.curr_M == 0)
	{
	  game1.kill_curr_holder();
	  game1.reset_circ();
	}
      else 
        {
	  cout << "Potatoe passed to " << game1.pass_potato()
	       << ", circle position " << game1.curr_holder 
	       << ".\n";
        }
    }
  cout << "The winner is number " << game1.winner() << '\n';
}
