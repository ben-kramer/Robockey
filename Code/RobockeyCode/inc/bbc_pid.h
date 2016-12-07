/* File: bbc_pid.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * PID structure
 */

#ifndef __BBC_PID__
#define __BBC_PID__

#include "m_general.h"
#include "bbc_definitions.h"

struct pid_t{

	float kp;
	float ki;
	float kd;

	float old_err;
	float old_val;
	float tot_err;

};

/*
 * Initialize a PID structure
 */
void init_pid(struct pid_t *pid, float kp, float ki, float kd);

/*
 * Update PID values
 */
float tick(struct pid_t *pid, float cur_val, float target);

#endif