/* File: main.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Header file for main robockey program.
 */

#include "main.h"

#define WLSS_CHANNEL 1
#define WLSS_MY_ADDRESS 0x10
#define WLSS_THEIR_ADDRESS 0x2C
#define WLSS_PACKET_LEN 10

/* All inputs are on port B */
#define PIN_COMM_TEST	0
#define PIN_PLAY 		1
#define PIN_GOAL_RED 	2
#define PIN_GOAL_BLUE 	3
#define PIN_PAUSE 		4
#define PIN_HALFTIME	5
#define PIN_GAME_OVER	6

// uncomment to see debug information over oosb
#define DEBUG_USB

typedef enum message {
	COMM_TEST, PLAY, GOAL_RED, GOAL_BLUE, PAUSE, HALFTIME, GAME_OVER,
	NONE
} Message;

Message last_message = NONE;

char score_red = 0;
char score_blue = 0;

char buf[WLSS_PACKET_LEN];

/* Takes a Message and converts its name to a string */
inline char* stringFromMessage(Message m) {
	static char* strings[] = {"Comm_test",
							   "Play",
							   "Score_red",
							   "Score_blue",
							   "Pause",
							   "Halftime",
							   "Game over",
							   "None"};

	return strings[m];
}

void init_wlss() {
	m_rf_open(WLSS_CHANNEL, WLSS_MY_ADDRESS, WLSS_PACKET_LEN);
}

/* Fills the buffer with 1 character */
void fill_buf(char f) {
	int i;

	for(i = 0; i < WLSS_PACKET_LEN; i++) {
		buf[i] = f;
	}
}

/* Sends the buffer over */
void send_message() {
	m_rf_send(WLSS_THEIR_ADDRESS, buf, WLSS_PACKET_LEN);
}

void send_comm_test() {
	fill_buf(0xA0);
	send_message();
}

void send_play() {
	fill_buf(0xA1);
	send_message();
}

void send_goal_red() {
	score_red++;

	fill_buf(0);

	buf[0] = 0xA2;
	buf[1] = score_red;
	buf[2] = score_blue;
	send_message();
}

void send_goal_blue() {
	score_blue++;

	fill_buf(0);

	buf[0] = 0xA3;
	buf[1] = score_red;
	buf[2] = score_blue;

	send_message();
}

void send_pause() {
	fill_buf(0xA4);
	send_message();
}

void send_halftime() {
	fill_buf(0xA6);
	send_message();
}

void send_game_over() {
	fill_buf(0xA7);
	send_message();
}

/* Sets all port B pins to input mode with the input pullup disabled */
void init_port_b() {
	DDRB &= 0;	/* Set port B to input mode */
	PORTB &= 0; /* Disable internal pullup resistor */
}

/* Initializes USB output */
void init_usb() {
    m_usb_init();
    while(!m_usb_isconnected());
}

/* Checks pins for button presses, and returns the button to be 
 * pressed, or NONE if no button is pressed. Buttons are prioritized
 * lowest-index to highest index (i.e. pressing 0 and 7 will return 0)
 */
Message check_pins() {
	Message res;

	/* Check what button was pressed */
	if(PINB & 1<<PIN_COMM_TEST) {
		res = COMM_TEST;
	} else if(PINB & 1<<PIN_PLAY) {
		res = PLAY;
	} else if(PINB & 1<<PIN_GOAL_RED) {
		res = GOAL_RED;
	} else if(PINB & 1<<PIN_GOAL_BLUE) {
		res = GOAL_BLUE;
	} else if(PINB & 1<<PIN_PAUSE) {
		res = PAUSE;
	} else if(PINB & 1<<PIN_HALFTIME) {
		res = HALFTIME;
	} else if(PINB & 1<<PIN_GAME_OVER) {
		res = GAME_OVER;
	} else {
		res = NONE;
	}

	#ifdef DEBUG_USB
	if(res != NONE) {
		m_usb_tx_string("Button pressed: ");
		//char *s = stringFromMessage(res);
		//m_usb_tx_string(s);
		m_usb_tx_string("\n");
	}
	#endif

	return res;
}

/* Checks PORTB for any inputs, and sends the appropriate
 * message when 1 button is pressed. */
void send_messages() {
	Message to_send = check_pins();

	if(to_send == NONE) {
		return;
	}

	switch(to_send) {
		case COMM_TEST: send_comm_test(); break;
		case PLAY: send_play(); break;
		case GOAL_RED: send_goal_red(); break;
		case GOAL_BLUE: send_goal_blue(); break;
		case PAUSE: send_pause(); break;
		case HALFTIME: send_halftime(); break;
		case GAME_OVER: send_game_over(); break;
		default: 
			#ifdef DEBUG_USB
			m_usb_tx_string("Bad enum: default case hit?!\n");
			#endif
			break;
	}

	/* Professional-level debouncing code */
	m_red(ON);
	m_wait(500);
	m_red(OFF);
}

/* Initialization functions */
void init(){
	m_disableJTAG();
	m_clockdivide(0);
	#ifdef DEBUG_USB
	init_usb();
	m_wait(3000);
	#endif

	m_bus_init();
	init_wlss();
	init_port_b();

	sei();
}	   

int main(void)
{
	init();

	while(1) {
		send_messages();
	}

}
