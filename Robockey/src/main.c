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
	OCR1B = 100;										//duty cycle
	TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);  //Enable Inturrupts
}  

void init_pwm_1(){
	DDRB |= (1<<0) | (1<<1) | (1<<2) | (1<<3);
	clear(PORTB,0);
	clear(PORTB,1);
	clear(PORTB,2);
	clear(PORTB,3);

}

void init(){
    m_disableJTAG();
    m_clockdivide(0);
    init_timer_1();
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

   	while(1)
   	{	
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
