#include <stdio.h>

int main()
{
  FILE *fp;
  while(1)
    {
      fp=fopen("temp", "w");
      fprintf(fp, "Testing...\n");
      fclose(fp);
    }

}
