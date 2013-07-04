#include <inttypes.h>

#ifdef UIK_PREEMPT
#include "TimerOne.h"
#endif

#include "UIK.h"

#define MAIN_TASK_PRIO	0
volatile UIK_tcb_t g_mainTcb;
unsigned char g_mainTaskStack[128]   = {0xE7};

#define TONE_TASK_PRIO	0
volatile UIK_tcb_t g_toneTcb;
unsigned char g_toneTaskStack[128]   = {0xE7};

#define MAIN_TASK2_PRIO	0
volatile UIK_tcb_t g_mainTcb2;
unsigned char g_mainTaskStack2[128]   = {0xE7};

#define WEB_TASK_PRIO	0
volatile UIK_tcb_t g_webTcb;
unsigned char g_webTaskStack[128]   = {0xE7};

#define SERVICE_TASK_PRIO	128
volatile UIK_tcb_t g_serviceTcb;
unsigned char g_serviceTaskStack[96]  = {0xE7};

volatile unsigned char  g_command = 0;
volatile UIK_event_t   g_event = {0};

void mainTask()
{
	UIK_sleep(500); //give time for rest of tasks to start
	for(;;)
	{
                led_update_delay2();
                //uik_counter();
		//g_command = random(255);
		//UIK_eventSet(&g_event,&g_command);
		//UIK_sleep(5000);
                #ifndef UIK_PREEMPT
                UIK_sleep(500);
                #endif
	}
}

void mainTask2()
{
	UIK_sleep(500); //give time for rest of tasks to start
	for(;;)
	{
                //led_hog();
		g_command = random(255);
		UIK_eventSet(&g_event,&g_command);
		//UIK_sleep(5000);
	}
}

void serviceTask()
{
	unsigned char* t_command = NULL;

	UIK_sleep(500);//give time for rest of tasks to start
	for(;;)
	{
                //led_hog();
                t_command = (unsigned char*)UIK_eventWaitFor(&g_event, 0);
		UIK_eventReset(&g_event);
		//Serial.print(t_command);
		//Serial.println();
	}
}

void setup()
{
        /*Let's toot our horn a little bit to celebrate the fact that we are starting up*/
        //tone();
  
        #ifdef UIK_PREEMPT
        Timer1.initialize(500000);         // initialize timer1, and set a 1/2 second period
        //Timer1.initialize(50000);
        Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
        Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
        #endif  
  
	Serial.begin(115200);

	UIK_init();
	UIK_eventReset(&g_event);
        
        //led_init();
        //counter_init();
        web_server_init();
        
        /*Let's toot our horn a little bit to celebrate the fact that we are starting up*/
        //tone();
        
	UIK_addTask(&g_serviceTcb, serviceTask,
		g_serviceTaskStack, sizeof(g_serviceTaskStack), SERVICE_TASK_PRIO);
        
	UIK_addTask(&g_mainTcb, mainTask,
		g_mainTaskStack, sizeof(g_mainTaskStack), MAIN_TASK_PRIO);
        /*
        UIK_addTask(&g_toneTcb, tone_loop,
		g_toneTaskStack, sizeof(g_toneTaskStack), TONE_TASK_PRIO);
        */
        UIK_addTask(&g_webTcb, web_server_loop,
		g_webTaskStack, sizeof(g_webTaskStack), WEB_TASK_PRIO);
        
	//UIK_addTask(&g_mainTcb2, mainTask2,
	//	g_mainTaskStack2, sizeof(g_mainTaskStack2), MAIN_TASK2_PRIO);
        
}


#ifdef UIK_PREEMPT
void callback()
{
  //UIK_reSchedule();
 
  //led_update_delay3();
 
  //g_command = random(255);
  //UIK_eventSet(&g_event,&g_command);
  
  UIK_sleep(1);
}
#endif


void loop()
{
	UIK_start(); //no return from here
}
