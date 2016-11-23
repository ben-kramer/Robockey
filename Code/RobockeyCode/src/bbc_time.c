/* File: bbc_time.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Time-keeping routines for robockey.
 */

#include "bbc_time.h"
#include "m_usb.h"

long time_ms = 0;

void time_isr() {
	time_ms++;
}

/* Initializes Timer3 */
void init_time() {

	/* Set timer to count up to OCR3A (mode 4)*/
	clear(TCCR3B, WGM33);
	set(TCCR3B, WGM32);
	clear(TCCR3A, WGM31);
	clear(TCCR3A, WGM30);

	/* Don't have the pin OC3A do anything on overflow */
	clear(TCCR3A, COM3A1);
	clear(TCCR3A, COM3A0);

	/* Enable the interrupt on TIMER3_COMPA(_vect) */
	set(TIMSK3, OCIE3A);
	set(TIFR3, OCF3A);

	/* Set OCR3A to the proper cycle count */
	OCR3A = TIMER_CYCLES_PER_MILLISEC;

	/* Set clock source to unprescaled clock */
	clear(TCCR3B, CS32);
	clear(TCCR3B, CS31);
	set(TCCR3B, CS30);
}

/* Returns the current time in ms */
long get_millis() {
	return time_ms;
}
