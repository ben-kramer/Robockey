/* File: bbc_puck.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu>, Cameron Zawacki,
 *		    Ben Bernstein
 *
 * Code for finding puck direction and sensing puck possesion
 */

#ifndef __BBC_PUCK__
#define __BBC_PUCK__

#include "m_general.h"
#include "bbc_definitions.h"

// 
void read_puck_values();

// Given an ADC value, record it to persistent array of values
void record_puck_adc(int index, int adc_val);

// Check the breakbeam ADC value compared to a threshold
int check_breakbeam();

// Take all ADC values and calculate current puck position
double calc_puck_direction();

#endif