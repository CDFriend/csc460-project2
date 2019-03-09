/**
 * \file LED_Test.h
 * \brief Constants and functions for controlling LEDs on a AT90USBKey
 * 
 * \mainpage Constants and functions for controlling the state of the onboard
 *  LEDs on the AT90USBKey. 
 *
 * \author Alexander M. Hoole
 * \date October 2006
 */

#define LED_B2_GREEN	0x20
#define LED_B2_RED		0x10
#define LED_B5_GREEN	0x40
#define LED_B5_RED		0x80
#define LED_B2			(LED_B2_GREEN | LED_B2_RED)
#define LED_B5			(LED_B5_GREEN | LED_B5_RED)

void enable_LED(unsigned int mask);
void init_LED_B2(void);
void init_LED_B5(void);
void disable_LEDs(void);
