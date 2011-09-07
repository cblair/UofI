#include <stdio.h>

struct token {
  int cat;
  char t;
  int ln;
};

int main()
{
  int *arr;
  int size = sizeof(int) * 2 + sizeof(char);
  arr = malloc(size);
  arr[0] = 2;
  printf("arr[0] = %d, size = %d\n", arr[0], size);
  arr = malloc(size);
  arr[1] = 3;
  printf("arr[0] = %d, arr[1] = %d, size = %d\n", arr[0], arr[1], size);

}
