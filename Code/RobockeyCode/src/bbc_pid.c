/* File: bbc_pid.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * PID Structure
 */

#include "bbc_pid.h"


void init_pid(struct pid_t *pid, float kp, float ki, float kd){
	pid -> old_err = 0;
	pid -> old_val = 0;
	pid -> tot_err = 0;
	pid -> kp = kp;
	pid -> ki = ki;
	pid -> kd = kd;
}

float tick(struct pid_t *pid, float cur_val, float target, float dt){
	float cur_err = cur_val - target;
	float err_dif = (cur_err - pid -> old_err)/dt;
	pid -> old_err  = cur_err;
	pid -> tot_err += cur_err*dt;
 
	return (pid -> kp)*(cur_err) + (pid -> ki)*(pid -> tot_err) + + (pid -> kd)*(pid -> err_dif);
}