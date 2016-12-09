/* File: bbc_m_rf.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu>, Cameron Zawacki,
 *		    Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Code for initializing mWii and getting the action sent over
 * wireless.
 */

#include "bbc_m_rf.h"

volatile int msg_count;
int old_msg_count;

/* Initializes mRF to listen and readies INT2_vect*/
void init_wlss(unsigned int my_addr) {
    // open mRF channel
    m_rf_open(WLSS_CHANNEL, my_addr, WLSS_PACKET_LEN);

    msg_count = 0;
    old_msg_count = 0;
}

/* Increments a counter when incoming packets are received */
inline void wlss_isr2() {
	msg_count++;
}

/* Checks if a message has come in, and if so loads the message.
 * Returns 0 if no new message was received, and 1 if a new 
 * message came in. If no new message comes in, does not change
 * the value in [m] */
int wlss_get_message(WirelessMessage *m) {
	/* If no new message came in, do nothing */
	if(msg_count == old_msg_count) {
		return 0;
	}

	old_msg_count++;

	unsigned char buf[WLSS_PACKET_LEN];
	m_rf_read(buf, WLSS_PACKET_LEN);

	/* Determine type of message sent */	
	switch(buf[0]) {
		case 0xA0:
			m->type = COMM_TEST; break;
		case 0xA1:
			m->type = PLAY; break;
		case 0xA2:
			m->type = GOAL_R;
			m->sr = buf[1];
			m->sb = buf[2];
			break;
		case 0xA3:
			m->type = GOAL_B;
			m->sr = buf[1];
			m->sb = buf[2];
			break;
		case 0xA4:
			m->type = PAUSE; break;
		case 0xA6:
			m->type = HALFTIME; break;
		case 0xA7:
			m->type = GAME_OVER; break;	
		default: 
			m->type = UNKNOWN;
			#ifdef DEBUG_USB
				// m_usb_tx_string("Bad character received! It was: ");
				// m_usb_tx_hex(buf[0]);
			#endif
			break;
	}
	return 1;
}
