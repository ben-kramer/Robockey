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
float theta_to_puck = 0.0;
float dist_to_puck = 0.0;
// Do we have possession of the puck (0 is no, 1 is yes)
int in_possession = 0;
// Do we trust our puck estimations
int puck_is_valid = 0;

void determine_strategy(int is_goalie, int defend_goal) {
	// Fill the localize script with the latest mWii reading
	get_mwii_reading();

	// Calculate bot localization state
	current_state = localize();
	print_localize(current_state);

	read_puck_values();
	theta_to_puck = calc_puck_direction();
	dist_to_puck = calc_puck_distance();
	puck_is_valid = found_puck();
	in_possession = check_breakbeam();

	if (is_goalie) {
		goalie_strategy();
	} else {
		if (in_possession) {
			drive_to_goal(defend_goal);
		} else {
			if (puck_is_valid) {
				drive_to_puck();
			} else {
				return_to_goal(defend_goal);
			}	
		}
	}
}

void goalie_strategy() {
	if (puck_is_valid && dist_to_puck < GOALIE_DIST_THRESH) {
		
	}
}

void drive_to_goal(int defend_goal) {
	// double goal_x = 115 * (1 - 2 * defend_goal); // Go to the goal on the opposite side
	// double goal_y = 0;
	// // Calculate the angle between the bot and the goal
	// // 0 to 360
	// double phi_to_goal = PI/2 - atan2((goal_y - current.y), (goal_x - current.x));
	// if (phi_to_goal < 0) {phi_to_goal = phi_to_goal + 2*PI;}

	// double delta_phi = phi_to_goal - current.phi;
	// if (fabs(delta_phi) < 0.08) {
	// 	set_motor_speeds(0.8, 0.8);
	// } else {
	// 	int spindir = 1 - 2 * (delta_phi > 180);
	// 	set_motor_speeds((0.5 + 0.1 * spindir), (0.5 - 0.1 * spindir));
	// }

	// double diff_scale = 
}

void drive_to_puck() {

}

void return_to_goal(int our_goal) {
	goal_x = 115 * (-1 + 2 * our_goal);
	goal_y = 0;

	double phi_to_goal = PI/2 - atan2((goal_y - current.y), (goal_x - current.x));
	if (phi_to_goal < 0) {phi_to_goal = phi_to_goal + 2*PI;}

	double delta_phi = phi_to_goal - current.phi;
	if (fabs(delta_phi) < 0.08) {
		set_motor_speeds(DRIVE_SPEED, DRIVE_SPEED);
	} else {
		int spindir = 1 - 2 * (delta_phi > 180);
		set_motor_speeds((0.5 + SPIN_DIFF * spindir), (0.5 - SPIN_DIFF * spindir));
	}
}