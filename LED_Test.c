#include <avr/io.h>
#include "LED_Test.h"
/**
 * \file LED_Test.c
 * \brief Small set of test functions for controlling LEDs on a AT90USBKey
 * 
 * \mainpage Simple set of functions to control the state of the onboard
 *  LEDs on the AT90USBKey. 
 *
 * \author Alexander M. Hoole
 * \date October 2006
 */

void init_LED_B2(void){
	//DDRB |= LED_B2;
	DDRB = 0xFF;
	PORTB = 0x00;
}

void init_LED_B5(void){
	//DDRB |= LED_B5;
	DDRB = 0xFF;
	PORTB = 0x00;
}

void enable_LED(unsigned int mask){
	PORTB = mask;
}

void disable_LEDs(void){
	PORTB = 0x00;
}
