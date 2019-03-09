#include <string.h>
#include "tta.h"
#include "LED_Test.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void Ping(void* param_ptr){
	init_LED_B5();
	enable_LED(LED_B5_GREEN);
	for(int x = 0; x < 1000; x++);
	disable_LEDs();
}

void Pong(void* param_ptr){
	init_LED_B2();
	enable_LED(LED_B2_GREEN);
	for(int x = 0; x < 1000; x++);
	disable_LEDs();
}

// Basic test that every 5 seconds turns on one of the wires temporarily.
void test1(){
		
	Scheduler_Init();
	Scheduler_StartPeriodicTask(0, 10, Ping, NULL);
	Scheduler_StartPeriodicTask(5, 10, Pong, NULL);
	Scheduler_Start();
}

// Basic test that adds two sporadic tests: one at the start and one 50 ms after start.
void test2(){
	Scheduler_Init();
	Scheduler_StartPeriodicTask(0, 10, Ping, NULL);
	Scheduler_AddSporadicTask(0, Pong, NULL);
	Scheduler_AddSporadicTask(50, Pong, NULL);
}

/**
 * Long periodic task (should run for >1ms).
 */
void LongPeriodicTask(void* arg)
{
    // Pull PB0 high (digital port 53)
    DDRB |= (1 << PB0);
    PORTB |= (1 << PB0);

    // Do a bunch of useless work
    unsigned int x;
    for (x = 0; x < 32000; x++);

    PORTB &= ~(1 << PB0);
}

/**
 * Long periodic tasks (>1ms) should not affect our timer values.
 */
void Test3(void)
{
	Scheduler_Init();
	Scheduler_StartPeriodicTask(0, 20, LongPeriodicTask, NULL);
	Scheduler_Start();
}

/*
The main function of the program. Will call a test function,
which sets up and runs a test.
*/
void main() {
	test1();
}
