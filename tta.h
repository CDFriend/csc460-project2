#include <stdint.h>

#ifndef TTA_H
#define TTA_H

#define MAXTASKS	8

typedef void (*voidfuncptr) (void);      /* pointer to void f(void) */ 

void Scheduler_Init();

void Scheduler_StartTask(int16_t delay, int16_t period, voidfuncptr task);

uint32_t Scheduler_Dispatch();

#endif
