/*UIK internal / hidden functions*/

/* 32 bit global variable that is incremented
 * by the tick handler. It is a count of the
 * number of ticks that have occured so far.
 */
uint32_t UIKTickNum = 0;

/* Variable that holds the the length of the 
 * tick, in microseconds.
 * TODO: precision? value?
 */
uint32_t UIKTickLen = 0;

/*This is the scheduler from class*/
void UIKDispatcher(void)
{
  
}


/*
 * Executers when there is nothing else to do. 
 * Could just be the lowest priority task.
 */
void UIKIdle(void)
{
  
}


/*Device driver for the UIK main tick timer*/
void UIKTickHandler(void)
{
  
}
