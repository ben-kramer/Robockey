/* File: main.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Header file for main robockey program.
 */

#ifndef __BBC_MAIN_H__
#define __BBC_MAIN_H__

#include "m_general.h"
#include "m_bus.h"
#include "m_imu.h"
#include "m_rf.h"
#include "m_usb.h"
#include "m_wii.h"
#include <math.h>
#include "bbc_m_wii.h"
#include "bbc_m_rf.h"
#include "bbc_m_usb.h"
#include "bbc_time.h"
#include "bbc_definitions.h"
#include "bbc_adc.h"
#include "bbc_puck.h"
#include "bbc_localize.h"
#include "bbc_drive.h"

// Code to run for qualification purposes
void qualify_mode();

// Run to determine which team we are defending (red or blue) and whether the bot is a goalie
void determine_start();

// Respond to the comm test
void comm_test();

#endif