#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "tta.h"
#include "LED_Test.h"

typedef struct {
	int16_t period;
	int16_t remaining_time;
	uint8_t is_running;
	voidfuncptr function_pointer;
	void* state_struct_ptr;
} periodic_task_t;

// Array of periodic tasks
periodic_task_t periodic_tasks[MAXTASKS];

// Last runtime for periodic tasks
uint16_t last_runtime_periodic;

typedef struct {
	int16_t delay;
	uint8_t ready_to_run;
	voidfuncptr function_pointer;
	void* state_struct_ptr;
} sporadic_task_t;

// Array of sporadic tasks
sporadic_task_t sporadic_tasks[MAXTASKS];

// Last runtime for sporadic tasks
uint16_t last_runtime_sporadic;

void Scheduler_Init(){
	last_runtime_periodic = millis();
	last_runtime_sporadic = millis();
}

void Scheduler_StartPeriodicTask(int16_t delay, int16_t period, voidfuncptr task, void* state_struct_ptr){
	
	static uint8_t id = 0;
	if (id < MAXTASKS){
		periodic_tasks[id].remaining_time = delay;
		periodic_tasks[id].period = period;
		periodic_tasks[id].is_running = 1;
		periodic_tasks[id].function_pointer = task;
		periodic_tasks[id].state_struct_ptr = state_struct_ptr;
		id++;
	}
}

uint16_t Scheduler_DispatchPeriodic(){
	
	uint16_t now = millis();
	uint16_t elapsed = now - last_runtime_periodic;
	last_runtime_periodic = now;
	voidfuncptr t = NULL;
	void* t_state_struct_ptr = NULL;
	uint16_t idle_time = 0xFFFF;
	
	// update each task's remaining time, and identify the first ready task (if there is one).
	for (uint8_t i = 0; i < MAXTASKS; i++){
		if (periodic_tasks[i].is_running){
			// update the task's remaining time
			periodic_tasks[i].remaining_time -= elapsed;
			if (periodic_tasks[i].remaining_time <= 0){
				if (t == NULL){
					// if this task is ready to run, and we haven't already selected a task to run,
					// select this one.
					t = periodic_tasks[i].function_pointer;
					t_state_struct_ptr = periodic_tasks[i].state_struct_ptr;
					periodic_tasks[i].remaining_time += periodic_tasks[i].period;
				}
				idle_time = 0;
			}
			else{
				idle_time = fmin(periodic_tasks[i].remaining_time, idle_time);
			}
		}
	}
	if (t != NULL){
		Disable_Interrupt();
		// If a task was selected to run, call its function.
		t(t_state_struct_ptr);
		Enable_Interrupt();
	}
	return idle_time;
}

void Set_Task_Period(uint8_t task_id, int16_t new_period){
	if periodic_tasks[task_id] != NULL{
		periodic_tasks[task_id].period = new_period;
	}
}

void Scheduler_AddSporadicTask(int16_t delay, voidfuncptr task, void* state_struct_ptr){
	for (uint8_t i = 0; i < MAXTASKS; i++) {
		// If we found an empty space for a task
		if (!sporadic_tasks[i].ready_to_run){
			sporadic_tasks[i].delay = delay;
			sporadic_tasks[i].function_pointer = task;
			sporadic_tasks[i].state_struct_ptr = state_struct_ptr;
			sporadic_tasks[i].ready_to_run = 1;
		}
	}
}

void Scheduler_DispatchSporadic(){
	uint16_t now = millis();
	uint16_t elapsed = now - last_runtime_sporadic;
	last_runtime_sporadic = now;
	voidfuncptr t = NULL;
	void* t_state_struct_ptr = NULL;
	
	// update each task's remaining time before running, and identify the first ready task (if there is one).
	for (uint8_t i = 0; i < MAXTASKS; i++){
		
		// If the task exists
		if (sporadic_tasks[i].ready_to_run){
			
			// update the task's remaining time
			sporadic_tasks[i].delay -= elapsed;
			
			// Set the sporadic task to run if it's ready
			if (sporadic_tasks[i].delay <= 0){
				t = sporadic_tasks[i].function_pointer;
				t_state_struct_ptr = sporadic_tasks[i].state_struct_ptr;
				
				// Indicate that this task cannot be run again
				sporadic_tasks[i].ready_to_run = 0;
			}
		}
	}
	
	if (t != NULL){
		// If a task was selected to run, call its function.
		Disable_Interrupt();
		t(t_state_struct_ptr);
		Enable_Interrupt();
	}
}

void Scheduler_Start(){
	while(1){
		uint16_t idle_time = Scheduler_DispatchPeriodic();
		if(idle_time){
			Scheduler_DispatchSporadic();
	}
}

// Interrupt vector which can only run when no periodic tasks are running.
SIGNAL(TIMER1_COMPB_vect) {
	Disable_Interrupt();
	
	init_LED_D2();
	enable_LED(LED_D2);
	
	for(unsigned int x = 0; x < 1000; x++);
	
	disable_LEDs();
	
	Enable_Interrupt();
}
