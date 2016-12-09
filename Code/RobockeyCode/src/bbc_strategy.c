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
int in_possession = 0;
// Do we trust our puck estimations
int puck_is_valid = 0;
motor_duty current_duty;


void determine_strategy(int is_goalie, int defend_goal) {
	// Fill the localize script with the latest mWii reading
	get_mwii_reading();

	// Calculate bot localization state
	cur_state = localize();
	// print_localize(cur_state);

	// read_puck_values();
	// print_puck_info();

	// theta_to_puck = calc_puck_direction();
	// dist_to_puck = calc_puck_distance();
	// puck_is_valid = found_puck();
	// in_possession = check_breakbeam();

	// if (is_goalie) {
	// 	goalie_strategy(defend_goal);
	// } else {
	// 	if (in_possession) {
	// 		drive_to_goal(defend_goal);
	// 	} else {
	// 		if (puck_is_valid) {
	// 			drive_to_puck();
	// 		} else {
	// 			return_to_goal(defend_goal);
	// 		}	
	// 	}
	// }
	// if (defend_goal) {
	// 	if (cur_state.x < 0) {
	// 		drive_to_goal(defend_goal);
	// 	} else {
	// 		float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, 0, 0);
	// 		// Set the motor duties
	// 		motor_duty current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, 0);
	// 		set_motor_speeds(current_duty.mL, current_duty.mR);
	// 	}
	// } else {
	// 	if (cur_state.x > 0) {
	// 		drive_to_goal(defend_goal);
	// 	} else {
	// 		float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, 0, 0);
	// 		// Set the motor duties
	// 		motor_duty current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, 0);
	// 		set_motor_speeds(current_duty.mL, current_duty.mR);
	// 	}
	// }
	int goal_x = 115 * (1 - 2 * defend_goal);
	int goal_y = 0;
	float phi_drive = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
	motor_duty duty_qualify;

	duty_qualify = calc_motor_diff(0.6, phi_drive, 0);
	// if (fabs(phi_drive - cur_state.phi) < 0.1) {
	// 	duty_qualify = calc_motor_diff(0.8, 0, 0);
	// } else {
	// 	duty_qualify = rotate_in_place(phi_drive);
	// }
	set_motor_speeds(duty_qualify.mL, duty_qualify.mR);
}
int goalie_range = 20;
void goalie_strategy(int our_goal) {
	float goal_x = 100 * (-1 + 2 * our_goal);
	float goal_y = 0;

	// if (puck_is_valid) {
	// 	if (dist_to_puck < GOALIE_DIST_THRESH) {
	// 		drive_to_puck();
	// 	} else {

	// 		if (fabs(cur_state.x - goal_x) < 5 && fabs(cur_state.y - 0) < 5) {
	// 			rotate_in_place(PI/2);
	// 		} else {
	// 			float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
	// 			set_motor_speeds(current_duty.mL, current_duty.mR);
	// 		}


	// 		float angle_to_puck = cur_state.phi - theta_to_puck;
	// 		if (angle_to_puck < PI/2) {
	// 			angle_to_puck = angle_to_puck - GOALIE_ANGLE_OFFSET;
	// 		} else {
	// 			angle_to_puck = angle_to_puck + GOALIE_ANGLE_OFFSET;
	// 		}
	// 		rotate_in_place(angle_to_puck);
	// 	}
	// } else {
	// 	if (fabs(cur_state.x - goal_x) < 5 && fabs(cur_state.y - 0) < 5) {
	// 		rotate_in_place(PI/2);
	// 	} else {
	// 		float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
	// 		// current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, in_possession);
	// 		set_motor_speeds(current_duty.mL, current_duty.mR);
	// 	}
	// }	

	if (fabs(cur_state.y - goalie_range) < 5) {
		// rotate_in_place(PI/2);
		goalie_range = -goalie_range;
	} else {
		// float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
		// current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, in_possession);
		set_motor_speeds(current_duty.mL, current_duty.mR);
	}
}

// What is the angle offset between the robot and the point we want to go to
float calc_delta_phi(float cX, float cY, float cPhi, int pX, int pY) {
	// Calculate the angle between the bot and the point
	float phi_to_point = PI/2 - atan2((pY - cY), (pX - cX));

	// -PI to PI
	float delta_phi = phi_to_point - cPhi;
	if (delta_phi < -PI) {delta_phi = delta_phi + 2*PI;}
	if (delta_phi > PI) {delta_phi = delta_phi - 2*PI;}
	return delta_phi;
}

motor_duty rotate_in_place(float phi_des) {
	motor_duty duty;
	duty.mL = 0.5;
	duty.mR = 0.5;
	float del_phi = phi_des - cur_state.phi;
	float unit_del_phi = del_phi / PI;
	duty.mL = 0.5 + (ROTATION_SPEED - 0.5) * unit_del_phi;
	duty.mR = 0.5 - (ROTATION_SPEED - 0.5) * unit_del_phi;
	return duty;
}

// Given an angle we have to turn, set the difference in motor duty
// proportional to the magnitude of the angle
motor_duty calc_motor_diff(float max_duty, float del_phi, int has_puck) {
	float mL_duty = 1.0;
	float mR_duty = 1.0; 
	float min_duty = (1 - max_duty);

	float unit_del_phi = del_phi / PI;

	motor_duty duty;

	if (has_puck && fabs(unit_del_phi) > MIN_RAD_DUTY_DIFF) {
		unit_del_phi = MIN_RAD_DUTY_DIFF * unit_del_phi / fabs(unit_del_phi);
	}
	if (unit_del_phi > 0) {
		mR_duty = 1.0 - unit_del_phi;
	} else {
		mL_duty = 1.0 + unit_del_phi;
	}
	

	duty.mL = min_duty + (max_duty - min_duty) * mL_duty;
	duty.mR = min_duty + (max_duty - min_duty) * mR_duty;

	return duty;
}

// Drive to the goal that we score on
void drive_to_goal(int our_goal) {
	// Determine which goal we should score on
	int other_goal = 1 - our_goal;
	int goal_x = 115 * (-1 + 2 * other_goal);
	// Don't go to the middle of the goal, go to the closest "goal post"
	int goal_y = GOAL_Y_POINT * (-1 + 2 * (cur_state.y > 0));
	// Find the angle we need to turn
	float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
	// Set the motor duties
	motor_duty current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, 1);
	set_motor_speeds(current_duty.mL, current_duty.mR);
}

void drive_to_puck() {
	motor_duty current_duty = calc_motor_diff(DRIVE_SPEED, theta_to_puck, 0);
	set_motor_speeds(current_duty.mL, current_duty.mR);
}

// Drive back towards the defended goal
void return_to_goal(int our_goal) {
	float goal_x = 80 * (-1 + 2 * our_goal);
	float goal_y = 20;
	motor_duty current_duty;
	if (fabs(cur_state.x - goal_x) < 5 && fabs(cur_state.y - goal_y) < 5) {
		rotate_in_place(PI/2);
	} else {
		float del_phi = calc_delta_phi(cur_state.x, cur_state.y, cur_state.phi, goal_x, goal_y);
		current_duty = calc_motor_diff(DRIVE_SPEED, del_phi, in_possession);
		set_motor_speeds(current_duty.mL, current_duty.mR);
	}
}