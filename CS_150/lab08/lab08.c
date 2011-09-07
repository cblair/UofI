#include <stdio.h>

int Factorial(int n)
{
  if (n < 2) return 1; /* base case */
  return (n * Factorial(n - 1));
}

void IntToAscii(int num)
{
  int prefix, currDigit;
  if (num < 10) putchar(num + '0'); /* print single char */
  else {
    prefix = num / 10; /* shift right one digit */
    IntToAscii(prefix); /* print shifted num */
    currDigit = num % 10;
    putchar(currDigit + '0'); /* print shifted digit */
  }
}

int main()
{
  int n, sum;
  char temp; //for newlines in input stream
  printf("Enter a number for a factorial: ");
  scanf("%d%c", &n);

  sum = Factorial(n);
  printf("Factorial: %d\n", sum);

  printf("IntToAscii: ");
  IntToAscii(n);

}
