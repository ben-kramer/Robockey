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
int yblob[4] = {368, 402, 367, 330};
int valid[4] = {1, 1, 0, 1};

int starA, starB, starC, starD;
int xA, yA, xB, yB, xC, yC, xD, yD, xCenter, yCenter;

int known_dist[6] = {13, 26, 20, 16, 29, 23};
// 	wrt 1-2			100 200 154 123 223 177
//  wrt 3-4			65	130	100	80	145 115

const_center three_stars() {
	int D12 = calc_dist(xblob[1], xblob[0], yblob[1], yblob[0]);
	int D23 = calc_dist(xblob[2], xblob[1], yblob[2], yblob[1]);
	int D13 = calc_dist(xblob[2], xblob[0], yblob[2], yblob[0]);

	int meas_dist[3] = {D12, D23, D13};
	int meas_ratio[3];
	int min_dist = meas_dist[min_ind(meas_dist, 3)];
	for (int i = 0; i < 3; ++i) {
		meas_ratio[i] = (1000*meas_dist[i]) / min_dist;
	}

	// 1-2-3 -> 100 200 223 = 523
	// 1-2-4 -> 100 177 123 = 400
	// 1-3-4 -> 181 125 100 = 406
	// 2-3-4 -> 130 100 115 = 345
	int ratio_sum = meas_ratio[0] + meas_ratio[1] + meas_ratio[2];
	if (ratio_sum > 4650) { // A-B-C
		int ind12 = min_ind(meas_ratio, 3);
		int ind13 = max_ind(meas_ratio, 3);
		int ind23 = 3 - ind12 - ind13;

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
		xA = xblob[starA];
		xC = xblob[starC];
		yA = yblob[starA];
		yC = yblob[starC];

		xCenter = (xA + xC)/2;
		yCenter = (yA + yC)/2;
		
		float theta = (float)atan2((yA-yC), (xA-xC)) - PI/2;
		if (theta < 0) {theta = theta + 2 * PI;}
		

	} else if (ratio_sum < 3750) { // 2-3-4
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

		xB = xblob[starB];
		xC = xblob[starC];
		yB = yblob[starB];
		yC = yblob[starC];
		
		xD = xblob[(3-starB-starC)];
		yD = yblob[(3-starB-starC)];
		
		float theta = atan2((yB-yC),(xB-xC)) + 1.10596;
		if (theta < 0) {theta = theta + 2 * PI;}
		int distBtoC = calc_dist(xB, xC, yB, yC);
		float distDtoA = (float) distBtoC * (16.0 / 26);
		xA = xD + distDtoA * cos(theta - .715);
		yA = yD + distDtoA * sin(theta - .715);

		xCenter = (xA + xC)/2;
		yCenter = (yA + yC)/2;
	}
	const_center out;
	out.xCent = xCenter;
	out.yCent = yCenter;
	out.th = theta;
	return out;
}

void four_stars() {
	/*
						starA

									starC
			starD





						starC


	*/

	int D12 = calc_dist(xblob[1], xblob[0], yblob[1], yblob[0]);
	int D23 = calc_dist(xblob[2], xblob[1], yblob[2], yblob[1]);
	int D34 = calc_dist(xblob[3], xblob[2], yblob[3], yblob[2]);
	int D41 = calc_dist(xblob[0], xblob[3], yblob[0], yblob[3]);
	int D13 = calc_dist(xblob[2], xblob[0], yblob[2], yblob[0]);
	int D24 = calc_dist(xblob[3], xblob[1], yblob[3], yblob[1]);

	int meas_dist[6] = {D12, D23, D34, D41, D13, D24};
	int meas_ratio[6];
	int min_dist = meas_dist[min_ind(meas_dist, 6)];
	for (int i = 0; i < 6; ++i) {
		meas_ratio[i] = (1000*meas_dist[i]) / min_dist;
	}

	int point0[6] = 	{1, 2, 3, 4, 1, 2};
	int point1[6] = 	{2, 3, 4, 1, 3, 4};

	int min_p0 = point0[min_ind(meas_ratio, 6)];
	int min_p1 = point1[min_ind(meas_ratio, 6)];
	int max_p0 = point0[max_ind(meas_ratio, 6)];
	int max_p1 = point1[max_ind(meas_ratio, 6)];

	// min = [4, 2]
	// max = [3, 4]

	if (min_p0 == max_p0) {starA = min_p0; starC = max_p1;}
	if (min_p0 == max_p1) {starA = min_p0; starC = max_p0;}
	if (min_p1 == max_p0) {starA = min_p1; starC = max_p1;}
	if (min_p1 == max_p1) {starA = min_p1; starC = max_p0;}

	int xA = xblob[starA-1];
	int xC = xblob[starC-1];
	int yA = yblob[starA-1];
	int yC = yblob[starC-1];

	int xCent = (xA + xC)/2;
	int yCent = (yA + yC)/2;

	float theta = (float)atan2((yA-yC), (xA-xC)) - PI/2;
	if (theta < 0) {theta = theta + 2 * PI;}
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