/* File: bbc_strategy.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Strategy controlling functions
 */

#include "bbc_strategy.h"

/* Latest x y and phi data */
loc_state current_state;

// Latest puck data
float delta_to_puck = 0.0;
float dist_to_puck = 0.0;
// Do we have possession of the puck (0 is no, 1 is yes)
int in_possession = 0;
// Do we trust our puck estimations
int puck_is_valid = 0;

void determine_strategy() {
	// Fill the localize script with the latest mWii reading
	get_mwii_reading();

	// Calculate bot localization state
	current_state = localize();

	read_puck_values();
	delta_to_puck = calc_puck_direction();
	dist_to_puck = calc_puck_distance();
	puck_is_valid = found_puck();
	in_possession = check_breakbeam();

	if (in_possession) {
		drive_to_goal();
	} else {
		if (puck_is_valid) {
			drive_to_puck();
		} else {
			return_to_goal();
		}
		
	}
}

void drive_to_goal() {

}

void drive_to_puck() {

}

void return_to_goal() {

}