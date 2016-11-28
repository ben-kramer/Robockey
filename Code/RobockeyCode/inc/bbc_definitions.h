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

/* ========================================================================== *
 * ========================= WIRELESS COMMUNICATION ========================= *
 * ========================================================================== */

#define WLSS_CHANNEL		1
#define WLSS_PACKET_LEN		10

/* ========================================================================== *
 * =========================== ADC Conversion Pins ========================== *
 * ========================================================================== */

#define BREAKBEAM_ADC_THRESH	500
#define ADC_MAX 1000
#define ADC_MIN 0

enum adc_pin_index {
	F0, F1, F4, F5, F6, F7, D4, B4, B5
};

#endif