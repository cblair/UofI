/*  calcFP.cpp

    A simple calculator using function pointers.

    Bruce M. Bolden
    March 31, 2005
*/

#include <iostream>

using namespace std;

   // Simple math functions
int Add( int a, int b )    { return a + b; }
int Sub( int a, int b )    { return a - b; }
int Mult( int a, int b )   { return a * b; }
int Divide( int a, int b ) { return a / b; }

int Evaluate( int a, int b, int (*pF)(int, int) );


int main()
{
    char op;
    int a, b;
    int result;
    cout << "Enter a postfix expression: " << flush;
    cin >> a >> b >> op;

    if( op == '+' )
       result = Evaluate( a, b, &Add );
    else if( op == '-' )
       result = Evaluate( a, b, &Sub );
    else if( op == '*' )
       result = Evaluate( a, b, &Mult );
    else if( op == '/' )
       result = Evaluate( a, b, &Divide );

    cout << a << " " << op << " " << b << " = " << result << endl;
}


/*  Evaluate a op b using a pointer to a function  */
int Evaluate( int a, int b, int (*pF)(int, int) )
{
    return pF( a, b );
} 
