#include <stdio.h>

int main(int argc, char* argv[])
{
  char b[2];
  b[0] = 'a';

  int **c = &b;
  int **d = &b[0];
  printf("c = %d, d = %d\n", c, d);
  return 0;
}
