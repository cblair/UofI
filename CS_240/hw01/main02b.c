#include <stdio.h>

int main()
{
  FILE *fp;
  int x = 0;
  while(x < 100000)
    {
      fp=fopen("temp", "w");
      fprintf(fp, "Testing...\n");
      fclose(fp);
      x++;
    }

}
