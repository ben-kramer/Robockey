/* File: bbc_strategy.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Strategy controlling functions
 */

#include "m_general.h"
#include "bbc_definitions.h"
#include "bbc_localize.h"
#include "bbc_drive.h"

typedef struct motor_duty {
	float mL;
	float mR;
} motor_duty;

void determine_strategy(int is_goalie, int defend_goal);

void goalie_strategy();

void drive_to_goal();

void drive_to_puck();

void return_to_goal(int our_goal);

float calc_delta_phi(float cX, float cY, float cPhi, int pX, int pY);