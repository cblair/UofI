/*  calcFP2.cpp

    A simple calculator using function pointers stored
    in a table.

    Bruce M. Bolden
    March 31, 2005
*/

#include <iostream>

using namespace std;

int Add( int a, int b ) { return a + b; }
int Sub( int a, int b ) { return a - b; }
int Mult( int a, int b ) { return a * b; }
int Divide( int a, int b ) { return a / b; }

struct opFuncEntry
{
    char op;
    int (*opFunc)(int, int);
};

struct opFuncEntry opFuncTable[] =
{
   {'+', &Add},
   {'-', &Sub},
   {'*', &Mult},
   {'/', &Divide}
};
const int opTableSize = sizeof(opFuncTable)/sizeof(opFuncEntry);

int Eval( char op, int a, int b );
int Evaluate( int a, int b, int (*f)(int, int) );



int main()
{
    char op;
    int a, b;
    int result;
    cout << "Enter a postfix expression: " << flush;
    cin >> a >> b >> op;

    result = Eval( op, a, b );

    cout << a << " " << op << " " << b << " = " << result << endl;
}

/*  Eval:  Evaluate expression using the opFuncTable */
int Eval( char op, int a, int b )
{
    int result;

    for( int i = 0 ; i < opTableSize ; i++ )
    {
       if( op == opFuncTable[i].op ) 
       {
           result = opFuncTable[i].opFunc( a, b) ;

               //  yet another way
           //result = Evaluate( a, b, opFuncTable[i].opFunc );
       }
    }

    return result;
}


/*  Evaluate a op b using a pointer to a function  */
int Evaluate( int a, int b, int (*f)(int, int) )
{
    return f( a, b );
} 
