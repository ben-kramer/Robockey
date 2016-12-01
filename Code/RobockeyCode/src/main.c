#include "main.h"

long last_toggle_ms = 0;
int playmode = 0;
int qualify = 1;

ISR(INT2_vect) {
	m_green(TOGGLE);
	wlss_isr2();
}

ISR(TIMER3_COMPA_vect) {
	time_isr();
}

ISR(ADC_vect){
    adc_isr();
}

/* Our wireless addresses are: 0x2C, 0x2D, 0x2E */
unsigned int my_addr = 0x2C;

double x_pos = -100;
double y_pos = 50;
double phi = 0;

/* The last message to come in */
WirelessMessage last_message;

/* Latest mWii data */
mWiiReading last_mWii;

/* Initialization functions */
void init(){
	m_disableJTAG();
	m_clockdivide(0);
	m_bus_init();
	m_wait(200);

	init_usb();
	m_wait(500);

	init_wlss(my_addr);
	init_time();
	init_adc();
	init_mwii();
	init_localize();

	sei();
}	   

int main(void)
{
	init();

	while(1) {

		/* Check for an incoming message. If one exists, print it */
		if(wlss_get_message(&last_message)) {
			// print_wlss_message(last_message);
			playmode = (last_message.type == PLAY);
		}

		get_mwii_reading(last_mWii);

		if (qualify) {
			
			double goal_x = 115 * (1 - 2 * (x_pos > 0)); // Go to the goal on the opposite side
			double goal_y = 0;
			// Calculate the angle between the bot and the goal
			// 0 to 360
			double phi_to_goal = 90 - 180 * atan2((goal_y - y_pos), (goal_x - x_pos)) / PI;
			if (phi_to_goal < 0) {phi_to_goal = phi_to_goal + 360;}

			double delta_phi = phi_to_goal - phi;
			if (fabs(delta_phi) < 5) {
				set_motor_speeds(0.8, 0.8);
			} else {
				int spindir = 1 - 2 * (delta_phi > 180);
				set_motor_speeds(0.5 * spindir, -0.5 * spindir)
			}
		}

		if(get_millis() - last_toggle_ms > 500) {
			m_red(TOGGLE);
			last_toggle_ms = get_millis();
		}
	}

}
