#include "uik_stack.h"

void loop()
{
  /*Init UIK*/
  UIKInitialize(500, 3);
  
  /*Add tasks*/
  UIKAddTask2();
  
  /*Run the current task*/
  UIKRun(TaskStack[UIKCurrentTaskI].tid);
  
}
