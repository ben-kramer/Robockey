/* File: bbc_puck.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu>, Cameron Zawacki,
 *		    Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Code for finding puck direction and sensing puck possesion
 */

#include "bbc_puck.h"
#include "bbc_definitions.h"

// Holds the latest ADC readings from the breakbeam sensor and the eight puck searching sensors
// 0 = Breakbeam reading
// 1-8 = Search readings going clockwise where 1 is straight ahead
int puck_ADC_values[9] = { 0 };
int pin_index = 0;

// Is the bot confident in it's puck estimate?
int valid_puck = 0;

// Cycle through all pins, call read_adc on each
void read_puck_values() {
	read_adc(pin_index);
	pin_index++;
	if (pin_index > 8) {
		pin_index = 0;
	}
	// print_adc_values(puck_ADC_values);
}

// Update the ADC array at the correct index
void record_puck_adc(int index, int adc_val) {
	puck_ADC_values[index] = 1024 - adc_val;
}

// Checks if the breakbeam ADC value is greater than a threshold value
// Returns 1 if the robot is in possession of the puck, 0 if not
int check_breakbeam() {
	if (puck_ADC_values[0] > BREAKBEAM_ADC_THRESH) {
		return 1;
	} else {
		return 0;
	}
}

// Calculates the angle of the direciton vector to the puck
// with respect to the direction vector of the robot.
// Returns angle in degrees
// 0 degrees is straight ahead, 90 is to the right of the robot
float calc_puck_direction() {
	if (puck_ADC_values[1] > STRAIGHT_IR_THRESH) {
		return 0.0;
	}
	float direction_sum = 0;
	float intensity_sum = 0;
	// Check each sensor reading
	int i = 1;
	int min_val = 1024;
	int min_val_index = 0;

	while (i < 9) {
		// Find the minimum ADC value and index.
		// This will be used as the temporary zero-position
		if (puck_ADC_values[i] < min_val) {
			min_val = puck_ADC_values[i];
			min_val_index = i;
		}
		i++;
	}

	// Start at the min, call its angle 0
	int j = min_val_index;
	int k = 1;
	while (j < 9) {
		int this_val = puck_ADC_values[j]; // Sanitize the ADC value
		// Normalize the reading compared to known min and max ADC values
		// This will give a decimal from 0 to 1
		float unit_intensity = (this_val - ADC_MIN) / (ADC_MAX - ADC_MIN);
		intensity_sum += unit_intensity;
		// Weighted average of the directions
		direction_sum += unit_intensity * (-180 + (45.0 * (k - 1)));
		j++;
		k++;
	}

	j = 1;
	while (j < min_val_index) {
		int this_val = puck_ADC_values[j]; // Sanitize the ADC value
		// Normalize the reading compared to known min and max ADC values
		// This will give a decimal from 0 to 1
		float unit_intensity = (this_val - ADC_MIN) / (ADC_MAX - ADC_MIN);
		intensity_sum += unit_intensity;
		// Weighted average of the directions
		direction_sum += unit_intensity * (-180 + (45.0 * (k - 1)));
		j++;
		k++;
	}

	// If the intensity sum is too low, the estimation is probably bad.
	valid_puck = (intensity_sum > INTENSITY_SUM_THRESH);
	
	float deg_from_min =  (direction_sum / intensity_sum) + 180;
	float deg_from_abs = deg_from_min + (45 * (min_val_index - 1)); // Convert back to absolute zero

	return deg_from_abs;
}

int found_puck() {return valid_puck;}

// Return measure of the distance to the puck
float calc_puck_distance() {
	int i;
	int max = 0;
	for (i = 1; i < 9; ++i) {
		if (puck_ADC_values[i] > max) {
			max = puck_ADC_values[i];
		}
	}
	// Return 1 - the max unit intensity
	// Low number = short distance
	return (1.0 - (max - ADC_MIN) / (ADC_MAX - ADC_MIN));
}