#include "m_general.h"
#include "m_usb.h"
#include "m_bus.h"
// #include "m_rf.h"
// #include "m_imu.h"
#include <math.h>

//#define DEBUG // uncomment to see debug information

#define PI 3.14
#define CLOCK 16000000
#define DELAY 1
#define CYCLES_20KHZ 800 
const unsigned char nextValue = 0;

volatile int pwm_count = 0;
volatile int not_pwm_count = 0;

/* ============================= INTERRUPTS ================================== */

ISR(TIMER1_COMPA_vect){
	pwm_count = 0;
	set(PORTB,0);
	clear(PORTB,1);
	set(PORTB,2);
	clear(PORTB,3);

}

ISR(TIMER1_COMPB_vect){
	not_pwm_count = 0;
	set(PORTB,1);
	clear(PORTB,0);	
	set(PORTB,3);
	clear(PORTB,2);
}

ISR(TIMER1_COMPC_vect){
	//pwm_count = 0;
}


/* ============================= INITS ================================== */

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

void init(){
    m_disableJTAG();
    m_clockdivide(0);
    init_async_pwm();
    m_bus_init();


    #ifdef DEBUG
    init_usb();
    m_wait(3000);
    #endif
 
 
    sei();
}   

/* ============================= MAIN LOOP ================================== */

int main(void)
{

	init();
	m_red(OFF);
	//set_left_duty(0.99);
	//set_right_duty(0.99);

	float duty = 0;

   	while(1)
   	{
   			set_left_duty(0.99);
   			set_right_duty(0.99);
   			m_red(ON);
   			m_wait(500);
   			m_red(OFF);
   			m_wait(500);

   			duty += 0.1;

   			if(duty > 1.0) {
   				duty = 0.0;
   			}
  //  		if(pwm_count < DELAY){
		// 	pwm_count++;
  //  		}
		// else if(pwm_count == DELAY){
		// 	pwm_count++;
		// 	set(PORTB,0);
	 //   	}
	 //   	if(not_pwm_count < DELAY){
	 //   		not_pwm_count++;
	 //   	}
		// else if(not_pwm_count == DELAY){
		// 	not_pwm_count++;
		// 	set(PORTB,1);
	 //   	}
   	}
}
