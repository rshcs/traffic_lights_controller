// lights.h

#ifndef _LIGHTS_h
#define _LIGHTS_h

#define YELLOW 1
#define GREEN 2
#define GREEN_2 4
#define RED 8

#define A_POINT 1
#define B_POINT 2
#define C_POINT 4

#define MUX_DELAY 3000
#define DISP_DELAY 10000

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class LightsClass
{
 private:
	 uint32_t disp_time = 0;
	 uint32_t disp_time_mux = 0;
	 int8_t current_state = 1;
	 int8_t current_mux_state = 1;
 public:
	 LightsClass(uint8_t init_val);

	 void led_out(uint8_t inVal);
	 void led_ctrl(uint8_t inVal);
	 void led_light(uint8_t ledNum, uint8_t ctrlNum);
	 void portl_config(uint8_t val1, uint8_t val2, uint8_t val3);
	
	 void state_config(uint16_t t1, uint16_t t2, uint16_t t3);
	 void time_config(uint16_t t1, uint16_t t2, uint16_t t3);
	 void mux_time_config(uint32_t dis_delay = 3);
	 void state_inc(uint16_t in_tme);
};

#endif

