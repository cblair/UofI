////////////////////////////////////////
//  CS 120 Sect. 3
//  Author:   Colby Blair
//  10/16/07
//  Lab 9
////////////////////////////////////////
/*
Names of files you have checked in:
  Fri Oct 12 01:16:38 2007   11940 blai4021.lab8-1.cpp
  Tue Oct 16 15:58:31 2007   10901 blai4021.lab9-1.cpp
End of list
*/

#include <iostream>

using namespace std;

class SmList
{
	private:
		static const int MAXSIZE = 6;
		int list[MAXSIZE];
		int list_size;
	public:
		SmList();
		int size();
		int max_size();
		bool empty();
		bool full();
		int front();
		int back();
		int at (int i);
		int push_front(int v);
		int pop_front();
		int push_back(int v);
		int pop_back();
		int set_at(int v, int i);
		void clear();
		void print(ostream& out = cout);
};

inline
ostream& operator<<( ostream& out, SmList& b)
{
  b.print( out );
  return ( out );
}

int main()
{
	SmList list, list2 = list;
	int x = 0;				//Temp int for return val.s
	int position, set_val;	  //Int.s for funct. calls that
                            //use val. and position info.
	cout << "************************************\n"
	<< "Begining of function test program\n"
	<< "************************************\n";

	//4. Calls function empty() to see if the list is 
	// empty. Prints accordingly.
	if(list.empty())
	  cout << "The list is empty!\n";
	else if (!list.empty()) 
	  cout << "The list is not empty!\n";	
	
	//9. Asks for a val to push into the list 6 times, 
	// and pushes it into the front. Then we ask one
	// more time to see what happens when it is full.
	for(int cnt = 1; cnt <= (list.max_size()); cnt++ )
	  {
	    cout << "Enter in a value to push into the front "
		    << "of the list:\n";
	    cin >> x;
	    x = list.push_front(x);
	    list.print();
	    cout << "Amount of available slots left: " << x << '\n';
	  }
	cout << "One more for the return val when full...\n";
	cin >> x;
	x = list.push_front(x);
	list.print();
	if(x == -1) cout << "x = -1, list is full\n";

	//13. and 8. Asks the user to set 6 val.s to any 6 list
  //elements. This is the users opportunity to test for 
  //what happens when they enter a index val for an
  //element that is not there. 
	for(int cnt = 1; cnt <= (list.max_size() ); cnt++)
	{
	  cout << "Enter in a). a value to set to b)."
		<< " a position: (" << cnt;
			if(cnt == 1) cout << "st";
			else if(cnt == 2) cout << "nd";
			else if(cnt == 3) cout << "rd";
			else cout << "th";
		cout << " request)\n";
	        cin >> set_val >> position;
		list.set_at(set_val, position);
		cout << "position = " << position
		<< " has a value of " << list.at(position) << '\n';
		list.print();
	}

	//2. Pretty simple.
	cout << "List size = " << list.size() << '\n';

	//3. Again.
	cout << "max_size() = " << list.max_size() << '\n';
	
	//4. Pretty simple/
	if(list.empty()) cout << "The list is empty!\n";
	else if (!list.empty()) cout << "The list is not empty!\n";

	//5. Just testing the function.
	if (list.full()) cout << "The list is full.\n";
	else if(!list.full()) cout << "The list is not full.\n";

	//6. etc.
	cout << "The value of the front element of the list is "
		<< list.front() << ".\n";

	//7. etc.
	cout << "The value of the back element of the list is "
		<< list.back() << ".\n";
	
	//15. etc.
	list.print();
	
	//10. Pops the value at the front out, and then shows remaining.
	// Then does it 6 more times to show what happens when the 
	// list goes empty. After every pop, we also test empty() and
	// full() too.
	cout << "Popping front of list... " << list.pop_front()
		<< " empty slot(s)in exist now.  List is now:\n";
	list.print();
	for(int cnt = 0; cnt < list.max_size(); ++cnt)
	  {
	    cout << "Popping front of list again... " << list.pop_front()
		<< " empty slot(s)in list now.  List is now:\n";
	    list.print();
	    if(list.empty())
	      cout << "The list is empty!\n";
	    else if (!list.empty()) 
	      cout << "The list is not empty!\n";	
	    if(list.full())
	      cout << "The list is full.\n";
	    else if (!list.full()) 
	      cout << "The list is not full\n";	
	  }
	
	//11. Pushes 6 mumbers onto the back of the list. Then does
	// 1 more to show what happens when the list is full. We also
	//print the empty slots returned back to x from the function.
	for(int cnt = 0; cnt <= (list.max_size() + 1); ++cnt)
	  {
	    cout << "Enter a value to push onto the back of the list: \n";
	    cin >> x;
	    x = list.push_back(x);
	    cout << "Amount of empty slots left: ";
	    if(x < 1) cout << "none\n";
	    else cout << x << '\n';
	    list.print();
	}
	cout << "One more to make sure the return value is -1 from "
	     << "a full list:\n";
	cin >> x;
	x = list.push_back(x);
	if(x == -1) cout << "Return value is " << x << ", list is full.\n";

	//12. Now we pop the val. of the back of the list 6 times,
	// giving us. We print the amount of slots left 6 times,
	// and test to see if x has the return val from the funct.
	// indication the list is empty.
	for(int cnt = 0; cnt <= (list.max_size() -1); ++cnt)
	  {
	    cout << "Popping off the back of the list...\n";
	    x = list.pop_back();
	    cout << "Done!  The amount of slots left is ";
	    if(x < 1) cout<< "none\n";
	    else cout << x << '\n';
	    list.print();
	  }

	//14. Now we use push_front to push on the val.s from
	// the count onto the list to create a simple list to
	// test clear on.
	cout << "Now rebuilding a default list...\n";
	for(int cnt = 0; cnt <= list.max_size(); ++cnt)
	{
	  list.push_front(cnt);
	  list.print();
	}
	list2 = list;
	cout << "List 2, per a copy constructor just used, now looks like this: \n";
	list2.print();

	cout << "Now clearing list again...\n";
	list.clear();
	
	//15. One last print
	cout << "Again, list one is:\n";
	list.print();
	cout << "...and List 2 (not cleared, after the first list is cleared) "
		<< "looks like:\n";
	list2.print();
}

//1. Default init.s all class data members as 0
SmList::SmList ()
	     :list_size(0)
{
  for(int i = 0; i < max_size(); ++i) list[i] = 0;
}

//2. Returns the member 'list size' when called
int SmList::size()
{		               		
	return (list_size);
}

//3. Init.s a local const. called MAXSIZE to 6,
//then returns it.
int SmList::max_size()
{
	return(MAXSIZE);
}

//4. Tests if funct. size returns a 0, and retruns 
// a 'true' if so. Otherwise, default to returning
// 'false'.
bool SmList::empty()
{
	if(list_size == 0) return (true);
	return (false);
}

//5. Tests if funct. size returns the same val. as
// funct. max_size.  If so, returns true for list
// is full. If not, the default return is false. 
bool SmList::full()
{
	if(max_size() == size()) return (true);
	return (false);	
}

//6. Tests if the empty funct. is true. If so, gives
// an error, and returns 0. Otherwise, just returns 
//the first element of the array list, list[0].
int SmList::front()
{
	if (empty()) 
	{
		cerr << "ERROR: SmList::front() - list is empty.\n";
		return (0);
	}
	return (list[0]);
}

//7. Tests if funct empty returns true. If so, the list is
// empty, and this funct. gives an error, and returns 0. 
//Otherwise, returns the last list element.
int SmList::back()
{
	if (empty()) 
	{
		cerr << "Error: SmList.back() - list is empty.\n";
		return (0);
	}
	return ( list[(max_size() -1)] );
}

//8. If i is negative, i is bigger than the corresponding list element,
// or i is 1 more than list size, then this function prints an error.
//Otherwise, it returns the corresponding list element to the index val.
int SmList::at(int i)
{
  if( (i < 0) || (i >= max_size()) || (list_size < i + 1 ) ) 
	  cerr << "ERROR: SmList::at() - index number is out of range "
	       << "or the index value is bigger than the amount of "
	       << "list elements\n";
	return (list[i]);
}

//9. If full is true, then it returns -1. Otherwise, it copies every
// element to the following element, and sets v to list_0. list_size
// is incremented by 1 everytime this happens to show that the list
// has grown. We then return the difference between max_size() and 
// size(), synonimous with how many open spot are left.
int SmList::push_front( int v )
{
	if(full()) return (-1);
  	for(int i = (max_size() - 1); i >= 1; --i) list[i] = list[i-1];
	list[0] = v;
	++list_size;
	return ( (max_size() - size()) );
}

//10. Tests if empty() is true. If so, returns the max_size().
// Otherwise, the val for list[0] is overwritten by list[1], and
// the rest are scooted down. 0's are set at the last list element
// and movedown, while list_size is decremented by 1. These work
// together to re-init. the list members and shrink the list.
// This funct. then returns how many elements are left, same as in the 
// previous function.
int SmList::pop_front()
{
  if(empty()) return (max_size());
  for(int i = 0; i <= (max_size() - 2); ++i) list[i] = list[i+1];
	list[(max_size() -1)] = 0;
	--list_size;
  return ( (max_size() - size()) );
}

//11. Test if full() is true. If so, returns -1.
// Otherwise, increments the list_size, sets v
// (the value to assign) to the first empty element
// with the set_at() function. Returns the number of
// remaining empty elements. 
int SmList::push_back(int v)
{
  if(full()) return (-1);
  ++list_size; 
  set_at(v, (list_size - 1));
  return ( (max_size() - size()) );
}

//12. Test if empty() is true. If so, it returns the max size.
// Otherwise, it sets the last list element that has had a 
// value added to it to 0. The the list is then shrunk to ignore
// that element. Then returns the number of remaining empty
// elements.
int SmList::pop_back()
{
  if(empty()) return (max_size());
  set_at(0, (list_size - 1));
  --list_size;
  return ( (max_size() - list_size) );
}

//13. If i is negative, or bigger than the last list element,
// we get an error, and return -1. Same with if i is greater than
// the list size. Otherwise, the corresponding element to the i 
// val is to the v val. 
int SmList::set_at(int v, int i)
{
 	if( (i < 0) || (i >= max_size()) )
	{
		cerr << "ERROR: SmList::set_at() - index value "
			<< i << " out of list bounds.\n";
		return (-1);
	}
	
	if(i > list_size)
	{
	 cerr << "Error: SmList::set_at() - index value is not the same as "
	      << "the first available element in the list (last element in "
	      << "list with a value).\n";
	 return(-1);
	}
	
  list[i] = v;
}

//14. A for loop sets the corresponding element to the count of 
// the loop to 0 until max size is reached. The list size is also
// set back to 0.
void SmList::clear()
{
  for(int i = 0; i < max_size(); ++i) set_at(i, 0);
  list_size = 0;
}

//15. Sets up a int called print_left for how many elements
// need to still be printed, by setting to the size of the 
// list. One element is printed, print_left is decremented 
// by 1, and then a comma is printed. The comma is done after
// the decrement because there will always be 1 less comma
// than the number of elements.
void SmList::print( ostream& out )
{  
	out << "[ ";
	for(int i = 0; i <= (list_size - 1); ++i)
  {
    cout << list[i]; 
    if( list_size > 1 && i < (list_size - 1) ) cout << ", ";
  }	
	out << " ]\n";
}
