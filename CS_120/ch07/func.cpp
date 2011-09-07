#include <iostream>

using namespace std;

unsigned long int square (int)

int main()
{
  int number1 = 0;
  unsigned long int numSquare;

  cout << "Enter a number to square:  ";
  cin >> number1;
  numSquare = square ( number1 );

  cout << The square of " << number1 << " is " << 
} 

unsigned long int square ( int number1 )
{
  unsigned long int square_to_return;

  number1 = number1 * number1;
  return (square_to_return);
}
