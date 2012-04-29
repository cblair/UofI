#ifndef _UIK_EXT_H
#define _UIK_EXT_H

/*
 * Have to define functions with funtion pointer args in .h files for Arduino.
 * Although we should be doing this for all of our functions.
 */
uint8_t UIKAddTask(void(*task)(void), uint8_t priority);

#endif
