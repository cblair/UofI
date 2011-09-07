#include <stdio.h>
#include "arr_sum.h"
#include "find_max.h"
#include "find_min.h"

const int SIZE = 10;
int arr[] = {5, 4, 2, 3, 1, 6, 0, 9, 7, 8};

int main()
{
  int x;
  for(x = 0; x < SIZE; x++)
    {
      printf("arr[%d] = %d\n", x, arr[x]);
    }

  printf("Sum = %d\n", arr_sum());
  printf("Max = %d\n", find_max());
  printf("Min = %d\n", find_min());

  return(0);
}
