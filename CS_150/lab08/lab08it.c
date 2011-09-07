/*This code works on gcc and lcc*/

#include <stdio.h>

int Factorial(int n)
{
  int i = n - 1;
  while(i > 1)
    {
      n = n * i;
      --i;
    } 
  return (n);
}

void IntToAscii(int num) 
{
  int place = 0;
  int temp = num;
  int mag = 0;
  int i = 0;
  if(num != 0) ++place;
  while( 0 != (temp /= 10)) 
    ++place;

  while(place > 0)
    {
      mag = 1;
      i = place - 1;
      while(i > 0)
	{
	  mag *= 10;
	  --i;
	}
      temp = num / mag;
      //print current digit
      putchar(temp + '0');
      num = num - (temp * mag);
      --place;
    }
  putchar('\n');
}

int main()
{
  int n, sum;
  char temp;
  printf("Enter a number for a factorial: ");
  scanf("%d%c", &n, &temp);
  
  sum = Factorial(n);
  printf("Factorial: %d\n", sum);

  printf("IntToAscii: ");
  IntToAscii(n);

}
