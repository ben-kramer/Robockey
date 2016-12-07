/* File: bbc_drive.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Printing routines for robockey.
 */

#include "bbc_drive.h"

const unsigned char nextValue = 0;

volatile int pwm_count = 0;
volatile int not_pwm_count = 0;

void t1_compa_isr() {
	pwm_count = 0;
	set(PORTB,0);
	clear(PORTB,1);
	set(PORTB,2);
	clear(PORTB,3);
}

void t1_compb_isr() {
	not_pwm_count = 0;
	set(PORTB,1);
	clear(PORTB,0);	
	set(PORTB,3);
	clear(PORTB,2);
}

void init_timer_1() 
{	

	TCCR1B |= (1<<CS10) | (1<<WGM12); 					//Set clock divider to 1 | Count up to OCR1A
	OCR1A = 800;										//20kHz
	OCR1B = 400;										//duty cycle
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);  //Enable Inturrupts
}  

void init_pwm_1(){
	DDRB |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
	clear(PORTB,0);
	clear(PORTB,1);
	clear(PORTB,2);
	clear(PORTB,3);

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

	/* Enable interrupts */
	set(TIMSK1, OCIE1A);

	/* 20 KHz PWM */
	OCR1A = CYCLES_20KHZ;

	/* Turn on big juicy clock */
	clear(TCCR1B, CS12);
	clear(TCCR1B, CS11);
	set(TCCR1B, CS10);
}

// Sets the motor speeds and directions
// Inputs are floats from -1 (backwards) to 1 (forwards)
void set_motor_speeds(float ml, float mr) {
	float left_duty = 0.5 + 0.5 * ml;
	float right_duty = 0.5 + 0.5 * mr;
	set_left_duty(left_duty);
	set_right_duty(right_duty);

	// print_motor_duty(left_duty, right_duty);
}