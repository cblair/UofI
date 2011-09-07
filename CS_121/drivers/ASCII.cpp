#include <iostream>
#include <fstream>

using namespace std;

//Stolen from http://www.cplusplus.com/reference/clibrary/ctime/clock.html
void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

int main()
{
    ofstream fout;
    fout.open("char_ASCII_int_values.txt");
    if(fout.fail())
    {
     cerr << "ERROR: file could not open.\n";
     exit(1);               
    }
    
    for(int i = 0; i < 256; ++i)
    {
     cout << static_cast<char>(i) << " = " << i << '\n';
     fout << static_cast<char>(i) << " = " << i << '\n';
    }
    wait(120);
}
