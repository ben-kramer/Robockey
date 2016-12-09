#include "main.h"

/* 	Things to check

	Localization
		x	Good mWii readings
		x	Angle == 0 when bot is facing blackboard
		x	Angle goes from -Pi to PI in radians clockwise
		x	X = 0, Y = 0 is center of the rink
		x	X < 0 is towards the door of Wu and Chen
		x	Y < 0 is towards the seats of Wu and Chen
		x	Max x is around 115 cm, max y is around 60 cm

	Puck-finding
		Good ADC data reading and writing
		ADC range doesn't saturate at max and min
		Accuracy of angle readings with uniform field
		Straight-IR-thresh is good
		Breakbeam thresh works
		Distance readings are reasonable

	Driving
		Drive forwards and backwards straight line
		Rotate in a circle
		Drive in a circle
		x	Set a speed limit?

	Strategy
		x	Is bot a goalie
		x	Which color are we defending
		Calculate min turning radius
		Write goalie strategy

*/
long last_toggle_ms = 0;

// Mode
int play_mode = 0;
int goalie = 0;

// Which team color are we
// Red = 0, Blue = 1
int color = 0;
// Which goal are we DEFENDING?
// Red goal = 0, Blue goal = 1
int goal_to_defend = 0;

ISR(INT2_vect) {
	wlss_isr2();
}

ISR(TIMER3_COMPA_vect) {
	time_isr();
}

ISR(ADC_vect){
    adc_isr();
}

/* Our wireless addresses are: 0x2C, 0x2D, 0x2E */
unsigned int my_addr = 0x2C;

/* Latest x y and phi data */
loc_state current;

/* The last message to come in */
WirelessMessage last_message;

/* Initialization functions */
void init(){
	m_red(ON);
	m_disableJTAG();
	m_clockdivide(0);
	m_bus_init();
	m_wait(200);

	init_usb();
	init_wlss(my_addr);
	init_time();
	init_adc();
	init_mwii();
	init_localize();
	init_drive();

	// Init color switch input on D3
	clear(DDRD, 7);
	set(PORTD, 7); // Enable pullup resistor
	// Init R/B LED outputs
	set(DDRB, 2);
	set(DDRB, 3);
	clear(PORTB, 2);
	clear(PORTB, 3);


	/* Initialize enable pins */
	DDRB |= 0x03;
	PORTB |= (0x03);

	sei();
}	   

int main(void)
{
	init();
	m_red(ON); // Made it to the loop
	m_green(OFF);
	determine_start();
	// comm_test();
	PORTB |= (0x03);

	while(1) {

		get_mwii_reading();
		current = localize();

		/* Check for an incoming message. */
		if(wlss_get_message(&last_message)) {
			play_mode = (last_message.type == PLAY);
			if (last_message.type == COMM_TEST) {comm_test();}
		}

		if (play_mode) {
			PORTB &= ~(0x03);
			m_green(ON); // Playing
			set_motor_speeds(0.8, 0.75); // Stop motors
		} else {
			PORTB |= (0x03);
			m_green(OFF);
			set_motor_speeds(0.5, 0.5);
		}

		if(get_millis() - last_toggle_ms > 500) {
			m_red(TOGGLE);
			last_toggle_ms = get_millis();
		}

		// print_main_loop(play_mode, goalie, goal_to_defend);
	}

}

// Run to determine which team we are defending (red or blue) and whether the bot is a goalie
void determine_start() {
	get_mwii_reading();
	current = localize();

	// Set our team's color
	if (!check(PIND,7)) {
		color = 1;
	}
	// If on the left  side, we are defending blue
	// If on the right side, we are defending red
	goal_to_defend = (int)(current.x > 0.0);
	// If the bot starts more than GOALiE_X_THRESH cm from the center, it is a goalie
	if (current.x < -GOALIE_X_THRESH || current.x > GOALIE_X_THRESH) {
		goalie = 1;
	}
}

// Flash the bicolor LED
void comm_test() {
	if (color) { // Blue
		set(PORTB, 2);
		m_wait(250);
		clear(PORTB, 2);
		m_wait(250);
		set(PORTB, 2);
		m_wait(250);
		clear(PORTB, 2);
		m_wait(250);
		set(PORTB, 2);
	} else { // Red
		set(PORTB, 3);
		m_wait(250);
		clear(PORTB, 3);
		m_wait(250);
		set(PORTB, 3);
		m_wait(250);
		clear(PORTB, 3);
		m_wait(250);
		set(PORTB, 3);
	}
}
