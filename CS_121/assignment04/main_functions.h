#ifndef _main_functions_h
#define _main_functions_h

#include <iostream>
#include <string>

using namespace std;

//This fuction first checks for correct input on the action,
//and cycles until something acceptable or 'exit' is typed.
//Then, dep_or_wit is set as false if it is a deposit measuring
//only the first character as either alpha 'd'; else, it is set
//as true if it is 'w' in the same fashion
void input(string name, string action, double amount, BQueue &line)
{
  bool dep_or_wit;
  cout << "TS: action[0] = " << action[0] << '\n';
  while(action[0] != 'd'
        && action[0] != 'D'
        && action[0] != 'w'
	&& action[0] != 'W')
    {
      cout << "Sorry-- we did not recognize your command-- "
           << "Deposit or Withdrawl, or exit?: "
        ;
      cin >> action;
      if(action == "exit") exit(1);
    }
  
  if(action[0] == 'd' || action[0] == 'D')
    dep_or_wit = false;
  else if(action[0] == 'w' || action[0] == 'W')
    dep_or_wit = true;
  
  line.push(name, dep_or_wit, amount);
}

BQueue shortest_line(BQueue &q1, BQueue &q2, BQueue &q3)
{
  if(q2.size() <= q1.size() && q2.size() <= q3.size())
    return(q2);
  if(q3.size() <= q1.size() && q3.size() <= q2.size())
    return(q3);
  
  return(q1);
}

void print_all(BQueue &q1, BQueue &q2, BQueue &q3)
{
  //find the max length of the list
  int max_length = 0;
  if(q2.size() > q1.size() && q2.size() > q3.size())
    max_length = q2.size();
  if(q3.size() > q1.size() && q3.size() > q2.size())
    max_length = q3.size();
  else max_length = q1.size();
  
  cout << "Line 1 (Express Line):\n";
  for(int i = 1; i <= q1.size(); ++i)
    q1.print_node(i);
  cout << '\n';
  
  cout << "Line 2:\n";
  for(int i = 1; i <= q2.size(); ++i)
    q2.print_node(i);
  cout << '\n';
  
  cout << "Line 3:\n";
  for(int i = 1; i <= q3.size(); ++i)
    q3.print_node(i);
  cout << '\n';
  
}


bool process_transactions(BQueue &q1,
			  BQueue &q2,
			  BQueue &q3,
			  Counter &counter1)
{
  sleep(1);
    
  cout << "TS: counter1.q1_counter = " <<  counter1.q1_counter << '\n';
  cout << "TS: counter1.q2_counter = " <<  counter1.q2_counter << '\n';
  cout << "TS: counter1.q3_counter = " <<  counter1.q3_counter << '\n';
  
  while(!q1.empty() || !q2.empty() || !q3.empty() )
    {
      
      if( counter1.q1_counter >= 2 && !q1.empty() )
	{
	  cout << "LINE 1 PROCESSED\n";
          cout << "TS: counter1.q1_counter = " <<  counter1.q1_counter << '\n';
          counter1.q1_counter = 0;
          q1.pop();
          return(false);
	}
      ++counter1.q1_counter;
      
      if( counter1.q2_counter >=5 && !q2.empty() )
	{
	  cout << "LINE 2 PROCESSED\n";
	  counter1.q2_counter = 0;
	  q2.pop();
	  return(false);
	}
      ++counter1.q2_counter;
      
      if( counter1.q3_counter >=5 && !q3.empty() )
	{
	  cout << "LINE 3 PROCESSED\n";
	  counter1.q3_counter = 0;
	  q3.pop();
	  return(false);
	}
      ++counter1.q3_counter;
      
      print_all(q1, q2, q3);
      return(true);
    }
  return(false);
}

void test_init(BQueue &q1, BQueue &q2, BQueue &q3)
{
  //Sally
  q1.push("Sally", false, 102.99);
  q1.push("Bob", false, 100.00);
  q1.push("Tim", false, 50.99);
  q1.push("Rufus", false, 20.00);
  
  q2.push("Jim", false, 150.00);
  q2.push("Alice", false, 100.00);
  
  q3.push("Beth", false, 10.99);
  q3.push("Liz", false, 15.00);
  
}

#endif
