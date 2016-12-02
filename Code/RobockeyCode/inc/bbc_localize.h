/* File: bbc_localize.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Localization code
 */

#ifndef __BBC_LOCALIZE__
#define __BBC_LOCALIZE__

#include "bbc_definitions.h"
#include "m_general.h"
#include "m_usb.h"

int calc_dist(int x2, int x1, int y2, int y1);

int min_ind(int array[], int size);

int max_ind(int array[], int size);

struct constellation three_stars(int x_vals[3], int y_vals[3]);

struct constellation four_stars(int x_vals[4], int y_vals[4]);

void localize();
void init_localize();
void update_blobs(mWiiReading *m);

struct loc_state {
	float x;
	float y;
	float phi;
};

struct constellation {
	float xCent;
	float yCent;
	float theta;
	float scale;
};

#endif