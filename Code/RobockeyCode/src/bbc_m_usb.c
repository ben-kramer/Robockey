/* File: bbc_m_usb.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Printing routines for robockey.
 */

#include "bbc_m_usb.h"
#include "bbc_localize.h"
#include "bbc_m_wii.h"

void init_usb() {
#ifdef DEBUG_USB
	m_usb_init();
	while(!m_usb_isconnected());
	m_wait(500);
	if(m_usb_isconnected()) {
		m_red(ON);
		m_green(ON);
		m_wait(500);
		m_red(OFF);
		m_green(OFF);
	}
#endif
}

void print_wlss_message(WirelessMessage m) {
#ifdef DEBUG_USB
	/* Print the message type */
	switch(m.type) {
		case COMM_TEST: 
			m_usb_tx_string("WirelessMessage is COMM_TEST.\n");
			break;
		case PLAY:
			m_usb_tx_string("WirelessMessage is PLAY.\n");
			break;
		case GOAL_R:
			m_usb_tx_string("WirelessMessage is GOAL_R.\n");
			m_usb_tx_string("\tScore Red is ");
			m_usb_tx_uint(m.sr);
			m_usb_tx_string("\n");
			m_usb_tx_string("\tScore Blue is ");
			m_usb_tx_uint(m.sb);
			m_usb_tx_string("\n");
			break;
		case GOAL_B:
			m_usb_tx_string("WirelessMessage is GOAL_B.\n");
			m_usb_tx_string("\tScore Red is ");
			m_usb_tx_uint(m.sr);
			m_usb_tx_string("\n");
			m_usb_tx_string("\tScore Blue is ");
			m_usb_tx_uint(m.sb);
			m_usb_tx_string("\n");
			break;
		case PAUSE:
			m_usb_tx_string("WirelessMessage is PAUSE.\n");
			break;
		case HALFTIME:
			m_usb_tx_string("WirelessMessage is HALFTIME.\n");
			break;
		case GAME_OVER:
			m_usb_tx_string("WirelessMessage is GAME_OVER.\n");
			break;
		case UNKNOWN:
			m_usb_tx_string("WirelessMessage is UNKNOWN.\n");
			break;
		default:
			m_usb_tx_string("Unrecognized WirelessMessage\n");
	}
#endif
}

void print_adc_values(int adc_array[9]) {
#ifdef DEBUG_USB
	m_usb_tx_string("\n Puck ADC Vals:");
	int i = 0;
	for (i = 0; i < 9; ++i) {
		m_usb_tx_string(" ");
		m_usb_tx_uint(adc_array[i]);
	}
#endif
}

void print_puck_info() {
#ifdef DEBUG_USB
	float angle = calc_puck_direction();
	float dist = calc_puck_distance();
	int valid = found_puck();

	m_usb_tx_string("\n Puck - Angle: ");
	m_usb_tx_int((int) (100*angle));
	m_usb_tx_string(" Dist: ");
	m_usb_tx_int((int) (100*dist));
	m_usb_tx_string(" Valid: ");
	m_usb_tx_int(valid);

#endif
}

void print_mWii_data(mWiiReading m) {
#ifdef DEBUG_USB
	int i;
	for (i = 0; i < 4; ++i) {
		m_usb_tx_string("\nmWii blobs - x");
		m_usb_tx_int(i);
		m_usb_tx_string(": ");
		m_usb_tx_int(m.data[i][0]);
		m_usb_tx_string(" y");
		m_usb_tx_int(i);
		m_usb_tx_string(": ");
		m_usb_tx_int(m.data[i][1]);	
		m_usb_tx_string(" v");
		m_usb_tx_int(i);
		m_usb_tx_string(": ");
		m_usb_tx_int(m.valid[i]);
	}
#endif
}

void print_constellation(constellation constel) {
#ifdef DEBUG_USB
	m_usb_tx_string("\nConst data - xC: ");
	m_usb_tx_int((int) constel.xCent);
	m_usb_tx_string(" yC: ");
	m_usb_tx_int((int) constel.yCent);
	m_usb_tx_string(" th: ");
	m_usb_tx_int((int) (constel.theta * 100.0));
	m_usb_tx_string(" scale: ");
	m_usb_tx_int((int) (constel.scale * 100));
#endif
}

void print_localize(loc_state current) {
#ifdef DEBUG_USB
	m_usb_tx_string("\nCurrent - xPos: ");
	m_usb_tx_int((int) current.x);
	m_usb_tx_string(" yPos: ");
	m_usb_tx_int((int) current.y);
	m_usb_tx_string(" phi*100: ");
	m_usb_tx_int((int) (current.phi * 100.0));
#endif
}

void print_motor_duty(float duty_l, float duty_r) {
#ifdef DEBUG_USB
	m_usb_tx_string("\nMotor duty (x100) - left: ");
	m_usb_tx_int((int) duty_l * 100);
	m_usb_tx_string(" right: ");
	m_usb_tx_int((int) duty_r * 100);
#endif
}

void print_main_loop(int play_mode, int goalie, int goal_to_defend) {
#ifdef DEBUG_USB
	m_usb_tx_string("\nLoop modes - play: ");
	m_usb_tx_int(play_mode);
	m_usb_tx_string(" goalie: ");
	m_usb_tx_int(goalie);
	m_usb_tx_string(" defended-goal: ");
	if (goal_to_defend) {
		m_usb_tx_string("blue");
	} else {
		m_usb_tx_string("red");
	}
#endif
}