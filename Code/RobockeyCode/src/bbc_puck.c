/* File: bbc_puck.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu>, Cameron Zawacki,
 *		    Ben Bernstein
 *
 * Code for finding puck direction and sensing puck possesion
 */

#include "bbc_puck.h"

// Holds the latest ADC readings from the breakbeam sensor and the eight puck searching sensors
// 0 = Breakbeam reading
// 1-8 = Search readings going clockwise where 1 is straight ahead
int puck_ADC_values[9] = { 0 };
int pin_index = 0;

// Cycle through all pins, call read_adc on each
void read_puck_values() {
	read_adc(pin_index);
	pin_index++;
	if (pin_index > 8) {
		pin_index = 0;
	}
}

// Update the ADC array at the correct index
void record_puck_adc(int index, int adc_val) {
	puck_ADC_values[index] = adc_val;
}

// Checks if the breakbeam ADC value is greater than a threshold value
// Returns 1 if the robot is in possession of the puck, 0 if not
int check_breakbeam() {
	if (puck_ADC_values[0] > BREAKBEAM_ADC_THRESH) {
		m_green(ON);
		return 1;
	} else {
		m_green(OFF);
		return 0;
	}
}

// Calculates the angle of the direciton vector to the puck
// with respect to the direction vector of the robot.
// Returns angle in degrees
// 0 degrees is straight ahead, 90 is to the right of the robot
double calc_puck_direction() {
	double direction_sum = 0;
	double intensity_sum = 0;
	// Check each sensor reading
	int i = 1;
	while (i < 9) {
		int this_val = puck_ADC_values[i]; // Sanitize the ADC value
		// Normalize the reading compared to known min and max ADC values
		// This will give a decimal from 0 to 1
		double unit_intensity = (double) (this_val - ADC_MIN) / (ADC_MAX - ADC_MIN);
		intensity_sum += unit_intensity;
		// Weighted average of the directions
		direction_sum += unit_intensity * (45.0 * (i-1));
		i++;
	}
	return direction_sum / intensity_sum;
}