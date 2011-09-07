    #include <stdio.h>   

void main()
{

  int intData;
  char chrData;
  char strData[100];
  float fltData;

  printf(" C++ Tutorial - Enter the integer data :");
  scanf("%d",&intData);
  printf(" C++ Tutorial - Enter the Character data :");
  scanf("%c",&chrData);
  printf(" C++ Tutorial - Enter the string data :");
  scanf("%s",strData);
  printf(" C++ Tutorial - Enter the float data :");
  scanf("%f",&fltData);

  printf("C++ Tutorial \nInteger Value: %d\nChar: %c\nString: %s\nFloat: %f\n",intData,chrData,strData,fltData);

}
