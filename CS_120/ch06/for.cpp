#include <iostream>

using namespace std;

int main ()
{

  int i, j, k, m;

  for (i = 1; i <= 5; i+= 2)
    {
      for (j = 1; j <= 4; j++ )
	{
	  k = i + j;
	  cout << "i= " << i << ", j= " << j << ", k= " << k << endl;
	}
      m = k + i; 
    }

}
