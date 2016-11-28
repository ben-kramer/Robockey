#include "main.h"

long last_toggle_ms = 0;

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

/* The last message to come in */
WirelessMessage last_message;

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

	sei();
}	   

int main(void)
{
	init();

	while(1) {

		/* Check for an incoming message. If one exists, print it */
		if(wlss_get_message(&last_message)) {
			//print_wlss_message(last_message);
		}
		
		read_adc(0);
		check_breakbeam();

		if(get_millis() - last_toggle_ms > 500) {
			m_red(TOGGLE);
			last_toggle_ms = get_millis();
		}
	}

}
