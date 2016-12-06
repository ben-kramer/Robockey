/* File: bbc_definitions.h
 *
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * All defines used within the robockey program.
 */

#ifndef __BBC_DEFINITIONS__
#define __BBC_DEFINITIONS__

/* Uncomment this to see debug information */
#define DEBUG_USB
#define PI 					3.14159265

#define CLOCK 16000000
#define DELAY 1

/* ========================================================================== *
 * ========================= WIRELESS COMMUNICATION ========================= *
 * ========================================================================== */

#define WLSS_CHANNEL		1
#define WLSS_PACKET_LEN		10

/* ========================================================================== *
 * =========================== ADC Conversion Pins ========================== *
 * ========================================================================== */

#define BREAKBEAM_ADC_THRESH	500
// If the front IR sensor is above this value, assume puck is at 0 degrees
#define STRAIGHT_IR_THRESH		800 
#define ADC_MAX 1024.0
#define ADC_MIN 0.0

enum adc_pin_index {
	F0, F1, F6, F5, F7, F4, D4, B4, B5
};

#endif