/* File: bbc_m_rf.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu>, Cameron Zawacki,
 *		    Ben Bernstein
 *
 * Code for initializing mWii and getting the message sent over
 * wireless.
 */

/* NOTE: If you want to expose the number of messages that have been
 * received, just include this file and "extern int msg_count". */

#ifndef __BBC_M_RF_H__
#define __BBC_M_RF_H__

#include "m_general.h"
#include "m_rf.h"
#include "m_usb.h"
#include "bbc_definitions.h"

typedef enum wlss_message_type {
	COMM_TEST, PLAY, GOAL_R, GOAL_B, PAUSE, HALFTIME, GAME_OVER, UNKNOWN
} WirelessMessageType;

/* Holds the message type and any auxiliary data (i.e. the score that
* comes along with the SA and SB messages) */
typedef struct wlss_message {
	WirelessMessageType type;
	unsigned int sr; /* Red-side score */
	unsigned int sb; /* blue-side score */
	char buf[WLSS_PACKET_LEN];
} WirelessMessage; 


/* Initializes mRF to listen and readies INT2_vect*/
void init_wlss(unsigned int my_addr);

/* Place this into ISR(INT2_vect) */
void wlss_isr2();

/* Place this into the main loop to read incoming messages. Returns 1
 * if a new message came in, 0 otherwise. If no message comes in, does
 * not change the value of [m]. */
int wlss_get_message(WirelessMessage *m);

#endif