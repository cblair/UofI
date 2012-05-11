#include <inttypes.h>
#include "adOS.h"

#define MAIN_TASK_PRIO	0
volatile ados_tcb_t g_mainTcb;
unsigned char g_mainTaskStack[128]   = {0xE7};

#define TONE_TASK_PRIO	0
volatile ados_tcb_t g_toneTcb;
unsigned char g_toneTaskStack[128]   = {0xE7};

#define MAIN_TASK2_PRIO	0
volatile ados_tcb_t g_mainTcb2;
unsigned char g_mainTaskStack2[128]   = {0xE7};

#define WEB_TASK_PRIO	0
volatile ados_tcb_t g_webTcb;
unsigned char g_webTaskStack[128]   = {0xE7};

#define SERVICE_TASK_PRIO	128
volatile ados_tcb_t g_serviceTcb;
unsigned char g_serviceTaskStack[96]  = {0xE7};

volatile unsigned char  g_command = 0;
volatile ados_event_t   g_event = {0};

void mainTask()
{
	ados_sleep(500); //give time for rest of tasks to start
	for(;;)
	{
                led_update_delay2();
                //uik_counter();
		g_command = random(255);
		ados_eventSet(&g_event,&g_command);
		//ados_sleep(5000);
                ados_sleep(500);
	}
}

void mainTask2()
{
	ados_sleep(500); //give time for rest of tasks to start
	for(;;)
	{
                //led_hog();
		g_command = random(255);
		ados_eventSet(&g_event,&g_command);
		ados_sleep(5000);
	}
}

void serviceTask()
{
	unsigned char* t_command = NULL;

	ados_sleep(500);//give time for rest of tasks to start
	for(;;)
	{
                //led_hog();
                t_command = (unsigned char*)ados_eventWaitFor(&g_event, 0);
		ados_eventReset(&g_event);
		//Serial.print(t_command);
		//Serial.println();
	}
}

void setup()
{
	Serial.begin(115200);

	ados_init();
	ados_eventReset(&g_event);
        
        //led_init();
        //counter_init();
        web_server_init();
        
        /*Let's toot our horn a little bit to celebrate the fact that we are starting up*/
        tone();
        
	ados_addTask(&g_serviceTcb, serviceTask,
		g_serviceTaskStack, sizeof(g_serviceTaskStack), SERVICE_TASK_PRIO);
        
	ados_addTask(&g_mainTcb, mainTask,
		g_mainTaskStack, sizeof(g_mainTaskStack), MAIN_TASK_PRIO);
        /*
        ados_addTask(&g_toneTcb, tone_loop,
		g_toneTaskStack, sizeof(g_toneTaskStack), TONE_TASK_PRIO);
        */
        ados_addTask(&g_webTcb, web_server_loop,
		g_webTaskStack, sizeof(g_webTaskStack), WEB_TASK_PRIO);
        
	//ados_addTask(&g_mainTcb2, mainTask2,
	//	g_mainTaskStack2, sizeof(g_mainTaskStack2), MAIN_TASK2_PRIO);
        
}

void loop()
{
	ados_start(); //no return from here
}
