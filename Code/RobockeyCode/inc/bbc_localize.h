/* File: bbc_localize.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein <bernsb@seas.upenn.edu>
 *
 * Localization code
 */

#ifndef __BBC_LOCALIZE__
#define __BBC_LOCALIZE__

#include "bbc_definitions.h"
#include "m_general.h"
#include "m_usb.h"
#include "bbc_m_wii.h"


typedef struct loc_state {
	float x;
	float y;
	float phi;
} loc_state;

typedef struct constellation {
	int xCent;
	int yCent;
	float theta;
	float scale;
} constellation;

int calc_dist(int x2, int x1, int y2, int y1);

int min_ind(int array[], int size);

int max_ind(int array[], int size);

constellation three_stars(int x_vals[3], int y_vals[3]);

constellation four_stars(int x_vals[4], int y_vals[4]);

loc_state localize();
void init_localize();
void update_blobs(mWiiReading m);

#endif