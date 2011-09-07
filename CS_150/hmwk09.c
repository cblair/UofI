#include <stdio.h>

int Smaller ( int x, int y )
{
  if ( x <= y )
    return x;
  else
    return y;
}

int main()
{
  int z = Smaller(4, 5);
  printf("z = %d\n", z);
}
