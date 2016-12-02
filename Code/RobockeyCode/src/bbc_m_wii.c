#include "bbc_m_wii.h"

/* Initializes mwii */
void init_mwii() {
	m_wii_open();
}

/* Gets mWii readings, and fills a struct with the data */
void get_mwii_reading(mWiiReading *m) {
	unsigned int buf[12];

	char res = m_wii_read(buf);

	/* If we get a bad reading, set all valid flags to 0 */
	if(!res) {
		int i; 
		for (i = 0; i < 4; i++) {
 			m->valid[i] = 0; 
 		}
	}

	/* If there wasn't an error, load the data */
	int i;
	for (i = 0;  i < 4; i++) {
		int x = buf[i*3];
		int y = buf[i*3+1];
		int s = buf[i*3+2];

		if(buf[x] == 1023 && buf[y] == 1023) {
			m->valid[i] = 0;
		}
		else {
			m->valid[i] = 1;
			m->data[i][0] = x;
			m->data[i][1] = y;
			m->data[i][2] = s; 
		}
	}
	// print_mWii_data(mWiiReading m);
	update_blobs(mWiiReading m);
	
}