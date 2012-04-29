#ifndef _UIK_STACK_H
#define _UIK_STACK_H

#include <avr/pgmspace.h>

#define MAX_TASKS 100

/* Index of current task being run*/
uint16_t UIKCurrentTaskI = 0;

/*Task State Type*/
enum task_state_t
{
	TREADY,
	TRUNNING,
	TWAITING
};


struct process_control_block
{
	/*
	 * Task pointer - function pointer to the task, 
	 * since our tasks are functions.
	 */
	void (*task_p)();

	/*Task state - see task_state enum type*/
	enum task_state_t task_state;

	/*Task/Process ID*/
	uint64_t tid;

	/*Program Counter*/
	uint64_t pc;

	/*Memory Limits - not used / supported*/
	/*uint64_t mem_ub;	//upper bound*/
	/*uint64_t mem_lb;	//lower bound*/

	/*Registers*/
	/*var types match the bit width of registers*/
	uint8_t stat_reg; 	/*Status Register*/
	uint8_t gen_reg;	/*General Purpose Register*/

	/*
	 * Opened Files - not implemented, since we don't
	 * have a file system
	 */
};

/*Task Stack - add elements here*/
struct process_control_block TaskStack[MAX_TASKS];

#endif
