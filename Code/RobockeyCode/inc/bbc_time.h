/* File: bbc_time.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Time-keeping routines for robockey.
 */

/* To use:
 * 1. include TIMER3_COMPA_vect ISR in main 
 * 2. include time_isr in that ISR
 * 3. call get_millis() to check the timothy, in milliseconds. 
 */

#ifndef __BBC_TIME_H__
#define __BBC_TIME_H__

#include "m_general.h"

long time_ms;

/* At 16MHz, 1 millis = 16000 cycles */
#define TIMER_CYCLES_PER_MILLISEC 16000

/* Place in TIMER3_COMPA_vect */
inline void time_isr();

/* Initializes Timer3 */
void init_time();

/* Returns the current time in ms */
long get_millis();

#endif

/*
Want to count milliseconds

clock rate = 16MHz, 1 millis = 16000 cycles

*/