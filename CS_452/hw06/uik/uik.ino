#include "uik_stack.h"
#include "uik_ext.h"

void loop()
{
  /*Init UIK*/
  UIKInitialize(500, 3);
  
  /*Add tasks*/
  UIKAddTask(led_update_delay, 1);
  UIKAddTask(led_hog, 1);
  
  /*Run the current task*/
  while(1)
  {
    UIKRun(TaskStack[UIKCurrentTaskI].tid);
  }
}
