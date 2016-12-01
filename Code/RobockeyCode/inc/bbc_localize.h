/* File: bbc_localize.h
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Localization code
 */

#ifndef __BBC_LOCALIZE__
#define __BBC_LOCALIZE__

typedef struct current {
	float x_pos;
	float y_pos;
	float phi;
};

typedef struct constellation {
	float xCent;
	float yCent;
	float th;
	float scale;
};

int calc_dist(int x2, int x1, int y2, int y1);

int min_ind(int array[6]);

int max_ind(int array[6]);

constellation three_stars(int x_vals[3], int y_vals[3]);

constellation four_stars(int x_vals[4], int y_vals[4]);

current localize();

#endif