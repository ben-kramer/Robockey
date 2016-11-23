/* File: bbc_m_wii.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Code to initialize and read data from mWii.
 */

#ifndef __BBC_M_WII_H__
#define __BBC_M_WII_H__

#include "m_wii.h"

/* Structure holding the data read in from the mWii, along
 * with flags indicating validity of a reading */
typedef struct mWiiReading {
	// Index 1 is the blob, index 2 is x, y, and size.
	// Bad readings have garbage values
	unsigned int data[4][3];

	// Indicates if reading n is valid. Check if a reading
	// is valid before using any data!
	int valid[4];
} mWiiReading;

/* Initializes m_wii */
void init_mwii();

/* Gets mWii readings and fills the struct with data */
void get_mwii_reading(mWiiReading *m);

#endif