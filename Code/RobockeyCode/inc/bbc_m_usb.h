/* File: bbc_m_usb.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Printing routines for robockey.
 */

#ifndef __BBC_M_USB_H__
#define __BBC_M_USB_H__

#include "m_general.h"
#include "m_usb.h"
#include "bbc_definitions.h"
#include "bbc_m_rf.h"
#include "bbc_m_wii.h"

/* Initializes usb. If usb initializes properly, the green and red 
 * leds blink once */
void init_usb();

void print_wlss_message(WirelessMessage m);

#endif
