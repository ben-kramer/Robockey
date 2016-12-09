/* File: bbc_adc.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Code to initialize the ADC and read/record data.
 */

#include "bbc_adc.h"
#include "bbc_definitions.h"

volatile int adc_flag = 0; // Has a conversion recently been completed
volatile int converting = 0; // Is the system currently performing a conversion
volatile int adc_index = 0; // The pin index that is currently being converted
volatile int adc_val = 0; // The resulting value for the specified pin

// Initialize ADC pins and system. To be run on first startup
void init_adc() {
	DDRF &= ~(1<<6|1<<7);
    PORTF &= ~(1<<6|1<<7);
 
    clear(ADMUX, REFS1); // Set ref voltage to Vcc
    set(ADMUX, REFS0);
  
    set(ADCSRA, ADPS2); // Set clock scale CHANGE IF clockdivide CHANGES!!!!!!
    set(ADCSRA, ADPS0); // Set clock scale CHANGE IF clockdivide CHANGES!!!!!!
  
    // Set for all ADC pins
    set(DIDR0, ADC0D); // Set as analog input F0
    set(DIDR0, ADC1D); // Set as analog input F1
    set(DIDR0, ADC4D); // Set as analog input F4
    set(DIDR0, ADC5D); // Set as analog input F5
    set(DIDR0, ADC6D); // Set as analog input F6
    set(DIDR0, ADC7D); // Set as analog input F7
    set(DIDR2, ADC8D); // Set as analog input D4
    set(DIDR2, ADC11D); // Set as analog input B4
    set(DIDR2, ADC12D); // Set as analog input B5

    set(ADCSRA, ADIE); // Enable interrupting
  
    clear(ADCSRA, ADEN); // Only change input pin when disabled
}

// Begin an ADC conversion of a specific port
// See bbc_definitions.h for enum of each pin
void start_adc(int index) {

	clear(ADCSRA, ADEN); // Only change input pin when disabled
    adc_index = index; // Record which pin is currently being converted

    // Set the input pin
    switch(index) { // There's probably a cleaner way to do this?
        case F0:
            clear(ADCSRB, MUX5);
            clear(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX0);
            break;
        case F1:
            clear(ADCSRB, MUX5);
            clear(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0);
            break;
        case F4:
            clear(ADCSRB, MUX5);
            set(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX0);
            break;
        case F5:
            clear(ADCSRB, MUX5);
            set(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            set(ADMUX, MUX0);
            break;
        case F6:
            clear(ADCSRB, MUX5);
            set(ADMUX, MUX2);
            set(ADMUX, MUX1);
            clear(ADMUX, MUX0);
            break;
        case F7:
            clear(ADCSRB, MUX5);
            set(ADMUX, MUX2);
            set(ADMUX, MUX1);
            set(ADMUX, MUX0);
            break;
        case D4:
            set(ADCSRB, MUX5);
            clear(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX0);
            break;
        case B4:
            set(ADCSRB, MUX5);
            clear(ADMUX, MUX2);
            set(ADMUX, MUX1);
            set(ADMUX, MUX0);
            break;
        case B5:
            set(ADCSRB, MUX5);
            set(ADMUX, MUX2);
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX0);
            break;
    }
  
    set(ADCSRA, ADEN); // Enable Conversion
    set(ADCSRA, ADSC); // Begin conversion
}

// Start, continue, or finish a conversion of a specific port
void read_adc(int index) {
    // If the conversion is still going or if it hasn't started
	if(!converting && !adc_flag){
        converting = 1;
        start_adc(index); // Begin a conversion
    }
    // If the conversion is done
    if(adc_flag){
        adc_flag = 0;
        record_puck_adc(adc_index, adc_val); // Record the converted value (see bbc_puck)
    }
    // If the conversion is in the process of running, don't do anything.
}

// Interrupt vector that triggers when conversion is complete
void adc_isr() {
    adc_flag = 1; // Mark as done
    converting = 0; // Mark as not converting
    adc_val = ADC; // Keep track of recorded value (matched with a specific pin)
    clear(ADCSRA, ADEN); // Disable conversion
}