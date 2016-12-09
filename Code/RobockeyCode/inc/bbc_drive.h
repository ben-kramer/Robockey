/* File: bbc_drive.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Drive controllers 
 */

#ifndef __BBC_DRIVE__
#define __BBC_DRIVE__

#include "m_general.h"
#include "bbc_definitions.h"
// #include "bbc_pid.h"

void init_drive();

void t1_compa_isr();

void t1_compb_isr();

void init_timer_1();

void init_pwm_1();

/* Sets duty cycle on B6 */
void set_left_duty(float duty);

/* Sets duty cycle on B7 */
void set_right_duty(float duty);

/* Initialize Timer1 PWM output on ports B6 (OC1B) and B7 (OC1C) */
void init_async_pwm();

void set_motor_speeds(float ml, float mr);

#endif