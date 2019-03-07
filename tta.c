#include "tta.h"
#include <avr/interrupt.h>
#include <math.h>
 
typedef struct
{
	int16_t period;
	int16_t remaining_time;
	uint8_t is_running;
	voidfuncptr functionPointer;
} task_t;
 
task_t tasks[MAXTASKS];
 
uint32_t last_runtime;
 
void Scheduler_Init()
{
	last_runtime = millis();
}
 
void Scheduler_StartTask(int16_t delay, int16_t period, voidfuncptr task)
{
	static uint8_t id = 0;
	if (id < MAXTASKS)
	{
		tasks[id].remaining_time = delay;
		tasks[id].period = period;
		tasks[id].is_running = 1;
		tasks[id].callback = task;
		id++;
	}
}
 
uint32_t Scheduler_Dispatch()
{
	uint8_t i;
 
	uint16_t now = millis();
	uint16_t elapsed = now - last_runtime;
	last_runtime = now;
	voidfuncptr t = NULL;
	uint16_t idle_time = 0xFFFF;
 
	// update each task's remaining time, and identify the first ready task (if there is one).
	for (i = 0; i < MAXTASKS; i++)
	{
		if (tasks[i].is_running)
		{
			// update the task's remaining time
			tasks[i].remaining_time -= elapsed;
			if (tasks[i].remaining_time <= 0)
			{
				if (t == NULL)
				{
					// if this task is ready to run, and we haven't already selected a task to run,
					// select this one.
					t = tasks[i].callback;
					tasks[i].remaining_time += tasks[i].period;
				}
				idle_time = 0;
			}
			else
			{
				idle_time = fmin(tasks[i].remaining_time, idle_time);
			}
		}
	}
	if (t != NULL)
	{
		// If a task was selected to run, call its function.
		Disable_Interrupts();
		t();
		Enable_Interrupts();
	}
	return idle_time;
}
