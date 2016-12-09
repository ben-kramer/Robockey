/* File: bbc_definitions.h
 *
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * All defines used within the robockey program.
 */

#ifndef __BBC_DEFINITIONS__
#define __BBC_DEFINITIONS__

/* Uncomment this to see debug information */
// #define DEBUG_USB
#define PI 					3.14159265

#define CLOCK 16000000
#define DELAY 1
#define CYCLES_20KHZ 800

/* ========================================================================== *
 * ========================= WIRELESS COMMUNICATION ========================= *
 * ========================================================================== */

#define WLSS_CHANNEL		1
#define WLSS_PACKET_LEN		10

/* ========================================================================== *
 * =========================== ADC Conversion Pins ========================== *
 * ========================================================================== */

// First index is the breakbeam
// Make this enum's order match the IR sensors going clockwise starting at the front
// i.e. 1 = front ir (0 degrees), 2 = 45 degrees clockwise, 3 90 degrees clockwise, etc.
enum adc_pin_index {
	// F0, F1, F6, F5, F7, F4, D4, B4, B5
	F7, B5, B4, F5, F6, F4, F1, D4, F0 
};

#define BREAKBEAM_ADC_THRESH	500
// If the front IR sensor is above this value, assume puck is at 0 degrees
#define STRAIGHT_IR_THRESH		950 
#define ADC_MAX 1024.0
#define ADC_MIN 0.0
#define INTENSITY_SUM_THRESH 	0.1

// mWii definitions

#define MWII_ANGLE_OFFSET		0 // in radians


/* ========================================================================== *
 * ================================ Strategy ================================ *
 * ========================================================================== */

#define GOALIE_X_THRESH			90
#define GOALIE_DIST_THRESH		0.5

#define DRIVE_SPEED				0.8f
#define GOALIE_DRIVE_SPEED		0.9
#define ROTATION_SPEED			0.6
#define GOALIE_ANGLE_OFFSET		0.2
#define MIN_RAD_DUTY_DIFF		0.1

#define GOAL_Y_POINT			10

#endif