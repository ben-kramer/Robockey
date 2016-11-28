/* File: bbc_adc.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Code to initialize and read data from ADC.
 */

#ifndef __BBC_ADC__
#define __BBC_ADC__

#include "m_general.h"
#include "bbc_definitions.h"

// First setup for ADC, run on startup
void init_adc();

// Begin an ADC conversion at a specific port (determined by the index)
void start_adc(int index);

// Store converted values into puck ADC array (see bbc_puck)
void read_adc(int index);

// Interrupt function
void adc_isr();

#endif