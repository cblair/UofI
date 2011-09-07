#include <time.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  clock_t start, end;
  double elapsed;

  start = clock();
  sleep(3);
  end = clock();
  elapsed = ((double) (start - end)) / CLOCKS_PER_SEC;

  printf("%f:%f:%f - %f\n", start, end, elapsed, CLOCKS_PER_SEC);
}
