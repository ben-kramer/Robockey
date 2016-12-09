/* File: bbc_drive.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Drive controllers 
 */

#include "bbc_drive.h"

const unsigned char nextValue = 0;

volatile int pwm_count = 0;
volatile int not_pwm_count = 0;

void init_drive() {
	init_async_pwm();
}

/* Sets duty cycle on B6 */
void set_left_duty(float duty) {
	if(duty < 0 || duty > 1) {
		return;
	}

	OCR1B = duty * OCR1A;
}

/* Sets duty cycle on B7 */
void set_right_duty(float duty) {
	if(duty < 0 || duty > 1) {
		return;
	}

	OCR1C = duty*OCR1A;
}

/* Initialize Timer1 PWM output on ports B6 (OC1B) and B7 (OC1C) */
void init_async_pwm() {
	/* Set Timer1 to PWM mode up to OCR1A */
	set(TCCR1B, WGM13);
	set(TCCR1B, WGM12);
	set(TCCR1A, WGM11);
	set(TCCR1A, WGM10);

	/* Configure B6 and B7 for output */
	DDRB |= 0xC0;

	/* Configure PWMs to go high during cycle time */
	set(TCCR1A, COM1B1);
	clear(TCCR1A, COM1B0);

	set(TCCR1A, COM1C1);
	clear(TCCR1A, COM1C0);

	/* 20 KHz PWM */
	OCR1A = CYCLES_20KHZ;

	/* Turn on big juicy clock */
	clear(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);
}

// Sets the motor speeds and directions
// Inputs are floats from 0 (backwards) to 1 (forwards)
void set_motor_speeds(float ml, float mr) {
	// float left_duty = 0.5 + 0.5 * ml;
	// float right_duty = 0.5 + 0.5 * mr;
	set_left_duty(ml);
	set_right_duty(mr);

	// print_motor_duty(ml, mr);
}