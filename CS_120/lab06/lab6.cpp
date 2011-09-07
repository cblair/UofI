#include <iostream>
#include <iomanip>

using namespace std;

int function1(int, int &);
int function2(int, int &);

int g = 4;

int main()
{
  int a = 11;
  int b = 56789;
  int x = 2020;

  cout << left;  // set left alignment (sticky)

  cout << "Main 1:   "
       << " g = " << setw(5) << g
       << " a = " << setw(6) << a
       << " b = " << setw(6) << b
       << " x = " << x;
  cout << "\n\n";

  a += 33;

  { // Begin local block
    cout << "Main 1a:  "
	 << setw(10 ) << ' ' // Only care about a
	 << " a = " << setw(6) << a;
    cout << "\n\n";

    int a = 777;
    g += 10;
    cout << "Main 2:   "
	 << " g = " << setw(5) << g
	 << " a = " << setw(6) << a
	 << " b = " << setw(6) << b
	 << " x = " << x;
    cout << "\n\n";
  } // END local block

  x = function1( a, b );
  cout << "Main 3:   "
       << " g = " << setw(5) << g
       << " a = " << setw(6) << a
       << " b = " << setw(6) << b
       << " x = " << x;
  cout << "\n\n";

  x = function2( a, b );
  cout << "Main 4:   "
       << " g = " << setw(5) << g
       << " a = " << setw(6) << a
       << " b = " << setw(6) << b
       << " x = " << x;
  cout << "\n\n";
} // END main()

int function1(int x, int &y)
{
  int a = x - y;
  g = x + y;
  y = a + g;
  cout << "function1:"
       << " g = " << setw(5) << g
       << " a = " << setw(6) << a
       << setw(11) << ' '  // no b in this scope
       << " x = " << setw(6) << x
       << " y = " << setw(6) << y;
  cout << "\n\n";
  return( a );
} // END function1()

int function2(int g, int &x)
{
  int temp = g;
  g = x;
  x = temp;

  temp = function1(g,x);
  g = temp * x;

  cout << "function2:"
       << " g = " << setw(5) << g
       << setw(11) << ' '  // no a in this scope
       << setw(11) << ' '  // no b in this scope
       << " x = " << setw(6) << x;
  cout << "\n\n";

  return( temp );
} // END function2()
