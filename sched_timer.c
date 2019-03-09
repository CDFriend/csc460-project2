/**
 * Timer routines for TTA scheduler.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "sched_timer.h"

#define Disable_Interrupt()		asm volatile ("cli"::)
#define Enable_Interrupt()		asm volatile ("sei"::)

static volatile int16_t g_timerTicks;

void Schedtimer_Init() 
{
    TCCR1B |= (1 << CS10);  // No prescaling
    TCCR1B |= (1 << WGM12);  // Set to CTC mode
    OCR1A = 16000;  // 1 msec = 16000 ticks
    TIMSK1 |= (1 << OCIE1A);  // Enable timer 0 compare interrupt

    // Init timer debug pin (PB0/Digital pin 53)
    DDRB = 0xFF;
    PORTB = 0x00;
}

int16_t millis()
{
    return g_timerTicks;
}

SIGNAL(TIMER1_COMPA_vect)
{
    PORTB = 0xFF;
    g_timerTicks++;
    PORTB = 0x00;
}
