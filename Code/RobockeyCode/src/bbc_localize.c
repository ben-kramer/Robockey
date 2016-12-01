/* File: bbc_localize.c
 * 
 * Authors: Ben Kramer <krab@seas.upenn.edu> Cameron Zawacki,
 *          Ben Bernstein
 *
 * Localization code
 */

// #include <math>
// #include "bbc_localize.h"

int xblob[4] = {837, 880, 919, 861};
int yblob[4] = {444, 412, 440, 509};
int valid[4] = {1, 1, 0, 1};

int starA, starB, starC, starD;
int xA, yA, xB, yB, xC, yC, xD, yD, xCenter, yCenter, theta;

int known_dist[6] = {13, 26, 20, 16, 29, 23};
// 	wrt 1-2			100 200 154 123 223 177
//  wrt 3-4			65	130	100	80	145 115

/*
					starA

								starC
		starD





					starC


*/	

// Determine the pixel center values of the constellation
// as well as the constellations rotation.
// theta is defined as counterclockwise rotation from "vertical" 
// (like on the assignment)
// Different logic for whether there are 3 or 4 stars
constellation three_stars(int x_vals[3], int y_vals[3]) {
	// Calculate pixel distances between stars
	int D12 = calc_dist(x_vals[1], x_vals[0], y_vals[1], y_vals[0]);
	int D23 = calc_dist(x_vals[2], x_vals[1], y_vals[2], y_vals[1]);
	int D13 = calc_dist(x_vals[2], x_vals[0], y_vals[2], y_vals[0]);

	int meas_dist[3] = {D12, D23, D13};
	int meas_ratio[3];
	int min_dist = meas_dist[min_ind(meas_dist, 3)];
	// Convert to ratios (distance / smallest distance)
	for (int i = 0; i < 3; ++i) {
		meas_ratio[i] = (1000*meas_dist[i]) / min_dist;
	}

	// Ratio sums for different combinations of 3 stars
	// Stars -> x-y	y-z	z-x
	// 1-2-3 -> 100 200 223 = 523
	// 1-2-4 -> 100 177 123 = 400
	// 1-3-4 -> 181 125 100 = 406
	// 2-3-4 -> 130 100 115 = 345
	int ratio_sum = meas_ratio[0] + meas_ratio[1] + meas_ratio[2];
	if (ratio_sum > 4650) { // A-B-C
		int ind12 = min_ind(meas_ratio, 3); // which ratio is stars 1-2 (smallest)
		int ind13 = max_ind(meas_ratio, 3); // which ratio is stars 1-3 (largest)
		int ind23 = 3 - ind12 - ind13;

		// If the order is 1-2, 1-3, 2-3, then the first star is star B,
		// the second is star A, and the third is starC
		if (ind12 == 0) {
			if (ind13 == 1) {
				starC = 2; starA = 1;
			} else {
				starC = 2; starA = 0;
			}
		} else if (ind13 == 0) {
			if (ind12 == 1) {
				starC = 0; starA = 1;
			} else {
				starC = 1; starA = 0;
			}
		} else {
			if (ind12 == 1) {
				starC = 0; starA = 1;
			} else {
				starC = 0; starA = 2;
			}
		}

		xA = x_vals[starA];
		xC = x_vals[starC];
		yA = y_vals[starA];
		yC = y_vals[starC];

		// Calculate the constellation center pixel
		xCenter = (xA + xC)/2;
		yCenter = (yA + yC)/2;
		
		// Rotation of the constellation (from the "vertical" picture online)
		theta = (float)atan2((yA-yC), (xA-xC)) - PI/2;
		if (theta < 0) {theta = theta + 2 * PI;}
		

	} else if (ratio_sum < 3750) { // 2-3-4
		// See above
		int ind34 = min_ind(meas_ratio, 3);
		int ind23 = max_ind(meas_ratio, 3);
		int ind24 = 3 - ind34 - ind23;

		if (ind34 == 0) {
			if (ind23 == 1) {
				starC = 1; starB = 2;
			} else {
				starC = 0; starB = 2;
			}
		} else if (ind23 == 0) {
			if (ind34 == 1) {
				starC = 1; starB = 0;
			} else {
				starC = 0; starB = 1;
			}
		} else {
			if (ind23 == 1) {
				starC = 2; starB = 1;
			} else {
				starC = 2; starB = 0;
			}
		}

		xB = x_vals[starB];
		xC = x_vals[starC];
		yB = y_vals[starB];
		yC = y_vals[starC];
		
		xD = x_vals[(3-starB-starC)];
		yD = y_vals[(3-starB-starC)];
		
		// 1.10596 is radian angle between stars 2-3 and the horizontal
		theta = atan2((yB-yC),(xB-xC)) + 1.10596;
		if (theta < 0) {theta = theta + 2 * PI;}

		// Find xA, yA from xD, yD
		int distBtoC = calc_dist(xB, xC, yB, yC);
		// distance from D to A is known ratio * distance from B to C
		float distDtoA = (float) distBtoC * (16.0 / 26);
		xA = xD + distDtoA * cos(theta - .715);
		yA = yD + distDtoA * sin(theta - .715);

		xCenter = (xA + xC)/2;
		yCenter = (yA + yC)/2;
	} else {
		return 0;
	}
	// return struct
	constellation out;
	out.xCent = xCenter;
	out.yCent = yCenter;
	out.th = theta;
	// Scale is cm / pixel, using known distance
	out.scale = 29.0/calc_dist(xC, xA, yC, yA);
	return out;
}

constellation four_stars(int x_vals[4], int y_vals[4]) {
	// Calculate distance values
	int D12 = calc_dist(x_vals[1], x_vals[0], y_vals[1], y_vals[0]);
	int D23 = calc_dist(x_vals[2], x_vals[1], y_vals[2], y_vals[1]);
	int D34 = calc_dist(x_vals[3], x_vals[2], y_vals[3], y_vals[2]);
	int D41 = calc_dist(x_vals[0], x_vals[3], y_vals[0], y_vals[3]);
	int D13 = calc_dist(x_vals[2], x_vals[0], y_vals[2], y_vals[0]);
	int D24 = calc_dist(x_vals[3], x_vals[1], y_vals[3], y_vals[1]);

	int meas_dist[6] = {D12, D23, D34, D41, D13, D24};
	int meas_ratio[6];
	int min_dist = meas_dist[min_ind(meas_dist, 6)];
	// Convert to ratios (distance / smallest distance)
	for (int i = 0; i < 6; ++i) {
		meas_ratio[i] = (1000*meas_dist[i]) / min_dist;
	}

	// distance key for how distances are calculated
	int point0[6] = 	{1, 2, 3, 4, 1, 2};
	int point1[6] = 	{2, 3, 4, 1, 3, 4};

	// find the min and max ratios, corresponds to 1-2 and 1-3
	// which distance was the smallest, and which stars were used for it?
	int min_p0 = point0[min_ind(meas_ratio, 6)];
	int min_p1 = point1[min_ind(meas_ratio, 6)];
	// which was the largest, and which stars were used.
	int max_p0 = point0[max_ind(meas_ratio, 6)];
	int max_p1 = point1[max_ind(meas_ratio, 6)];

	// min = [min_p0, min_p1], max = [max_p0, max_p1]
	// The ones that are the same are star A,
	// Star C is the other one in max

	if (min_p0 == max_p0) {starA = min_p0; starC = max_p1;}
	if (min_p0 == max_p1) {starA = min_p0; starC = max_p0;}
	if (min_p1 == max_p0) {starA = min_p1; starC = max_p1;}
	if (min_p1 == max_p1) {starA = min_p1; starC = max_p0;}

	int xA = x_vals[starA-1];
	int xC = x_vals[starC-1];
	int yA = y_vals[starA-1];
	int yC = y_vals[starC-1];

	// calculate
	int xCent = (xA + xC)/2;
	int yCent = (yA + yC)/2;

	float theta = (float)atan2((yA-yC), (xA-xC)) - PI/2;
	if (theta < 0) {theta = theta + 2 * PI;}

	constellation out;
	out.xCent = xCenter;
	out.yCent = yCenter;
	out.th = theta;
	// Scale is cm / pixel, using known distance
	out.scale = 29.0/calc_dist(xC, xA, yC, yA);
	return out;
}

// Calculate current position and orientation
current localize() {
	int valid_sum = valid[0] + valid[1] + valid[2] + valid[3];
	int good_x[valid_sum];
	int good_y[valid_sum];
	for (int i = 0; i < 4; ++i) {
		if (valid[i]) {
			good_x[i] = xblob[i];
			good_y[i] = yblob[i];
		}
	}
	constellation const_data;
	if (valid_sum == 3) {
		const_data = three_stars(good_x, good_y);
	} else if (valid_sum == 4) {
		const_data = four_stars(good_x, good_y);
	}
}

int calc_dist(int x2, int x1, int y2, int y1) {
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

int min_ind(int array[], int size) {
	int min = array[0];
	int ind = 0;
	for (int i = 1; i < size; ++i) {
		if (array[i] < min) {
			min = array[i];
			ind = i;
		}
	}
	return ind;
}
int max_ind(int array[], int size) {
	int max = array[0];
	int ind = 0;
	for (int i = 1; i < size; ++i) {
		if (array[i] > max) {
			max = array[i];
			ind = i;
		}
	}
	return ind;
}