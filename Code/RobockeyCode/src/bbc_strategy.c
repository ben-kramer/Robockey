/* File: bbc_strategy.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Strategy controlling functions
 */

#include "bbc_strategy.h"

/* Latest x y and phi data */
loc_state cur_state;

// Latest puck data
float theta_to_puck = 0.0;
float dist_to_puck = 0.0;
// Do we have possession of the puck (0 is no, 1 is yes)
int in_possession = 1;
// Do we trust our puck estimations
int puck_is_valid = 0;

void determine_strategy(int is_goalie, int defend_goal) {
	// Fill the localize script with the latest mWii reading
	get_mwii_reading();

	// Calculate bot localization state
	cur_state = localize();
	// print_localize(cur_state);

	read_puck_values();
	theta_to_puck = calc_puck_direction();
	// dist_to_puck = calc_puck_distance();
	// puck_is_valid = found_puck();
	// in_possession = check_breakbeam();

	if (is_goalie) {
		goalie_strategy();
	} else {
		if (in_possession) {
			drive_to_goal(defend_goal);
		} else {
			if (puck_is_valid) {
				drive_to_puck();
			} else {
				// m_usb_tx_int(1);
				return_to_goal(defend_goal);
			}	
		}
	}
}

void goalie_strategy() {
	if (puck_is_valid && dist_to_puck < GOALIE_DIST_THRESH) {
		
	}
}

float calc_delta_phi(float cX, float cY, float cPhi, int pX, int pY) {
	// Calculate the angle between the bot and the point
	float phi_to_point = PI/2 - atan2((pY - cY), (pX - cX));

	// -PI to PI
	float delta_phi = phi_to_point - cPhi;
	if (delta_phi < -PI) {delta_phi = delta_phi + 2*PI;}
	if (delta_phi > PI) {delta_phi = delta_phi - 2*PI;}
	return delta_phi;
}

motor_duty calc_motor_diff(float max_duty, float del_phi, int has_puck) {
	float mL_duty = 1.0;
	float mR_duty = 1.0; 
	float min_duty = (1 - max_duty);

	float unit_del_phi = del_phi / PI;

	motor_duty duty;

	// if (has_puck) {

	// } else {
		if (unit_del_phi > 0) {
			mR_duty = 1.0 - unit_del_phi;
		} else {
			mL_duty = 1.0 + unit_del_phi;
		}
	// }

	duty.mL = min_duty + (max_duty - min_duty) * mL_duty;
	duty.mR = min_duty + (max_duty - min_duty) * mR_duty;

	return duty;
}

// Drive to the goal that we score on
void drive_to_goal(int our_goal) {
	int other_goal = 1 - our_goal;
	int goal_x = 115 * (-1 + 2 * other_goal);
	int goal_y = 0;
	float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
	motor_duty current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, in_possession);
	set_motor_speeds(current_duty.mL, current_duty.mR);
}

void drive_to_puck() {
	// float del_phi = calc_puck_direction();
	float del_phi = theta_to_puck;
	motor_duty current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, in_possession);
	set_motor_speeds(current_duty.mL, current_duty.mR);
}

// Drive back to the defended goal
void return_to_goal(int our_goal) {
	float goal_x = 90 * (-1 + 2 * our_goal);
	float goal_y = 0;
	float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
	motor_duty current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, in_possession);
	set_motor_speeds(current_duty.mL, current_duty.mR);
}