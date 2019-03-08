#ifndef TTA_H
#define TTA_H

#include <stdint.h>
// The maximum number of tasks
#define MAXTASKS	8

#define Disable_Interrupt()		asm volatile ("cli"::)
#define Enable_Interrupt()		asm volatile ("sei"::)

typedef void (*voidfuncptr) (void* ptr);      /* pointer to void f(void* ptr) */ 

void Scheduler_Init();

void Scheduler_StartPeriodicTask(int16_t delay, int16_t period, voidfuncptr task, void* state_struct_ptr);

// Selects a periodic task to run.
uint16_t Scheduler_DispatchPeriodic();

/*
Changes a task's period.
Does not go into effect until the next time it runs.
task_id is its index in the array of tasks.
The first task created will have task_id = 0, second task has task_id = 1, etc.
*/
void Set_Task_Period(uint8_t task_id, int16_t new_period);

// Adds a sporadic task to run during the idle period.
void Scheduler_AddSporadicTask(int16_t delay, voidfuncptr task, void* state_struct_ptr);

void Scheduler_DispatchSporadic();

// Starts the scheduler. Serves as the main loop of the OS.
void Scheduler_Start();

#endif
