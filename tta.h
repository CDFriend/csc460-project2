#ifndef TTA_H
#define TTA_H

// The maximum number of tasks, not including the e
#define MAXTASKS	8

#define Disable_Interrupt()		asm volatile ("cli"::)
#define Enable_Interrupt()		asm volatile ("sei"::)

typedef void (*voidfuncptr) (void);      /* pointer to void f(void) */ 

void Scheduler_Init();

void Scheduler_StartTask(int16_t delay, int16_t period, voidfuncptr task);

uint16_t Scheduler_Dispatch();

#endif
