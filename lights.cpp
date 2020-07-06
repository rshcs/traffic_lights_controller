
#include "lights.h"

LightsClass::LightsClass(uint8_t init_val)
{
	DDRL = 0xFF;
	DDRC = 0xFF;
	DDRA = 0xFF;

	PORTL = 0b00001111;
	PORTC = 0xFF;
	PORTA = 0;

	disp_time = millis();
	disp_time_mux = micros();
}

void LightsClass::led_out(uint8_t inVal)
{
	inVal = ~inVal & 0b00001111;
	uint8_t port_val = PORTL & 0b11110000;
	port_val |= inVal;
	PORTL = port_val;
}

void LightsClass::led_ctrl(uint8_t inVal)
{
	uint8_t port_val = PORTL & 0b00001111;
	port_val |= inVal << 4;
	PORTL = port_val;
}

void LightsClass::led_light(uint8_t ledNum, uint8_t ctrlNum)
{
	led_out(ledNum);
	led_ctrl(ctrlNum);
}

void LightsClass::portl_config(uint8_t val1, uint8_t val2, uint8_t val3)
{
	/*
	PORTL = val1;  // <= A
	delay(3);
	PORTL = val2;  // <= B
	delay(3);
	PORTL = val3;  // <= C
	delay(3);
	*/
	
	if (current_mux_state == 1)
	{
		PORTL = val1;  // <= A
	}
	else if (current_mux_state == 2)
	{
		PORTL = val2;  // <= B
	}
	else if (current_mux_state == 3)
	{
		PORTL = val3;  // <= C
	}
	
	
}

void LightsClass::state_config(uint16_t t1, uint16_t t2, uint16_t t3)
{
	if (current_state == 1)
	{
		portl_config(0x19, 0x23, 0x47); // st1
	}
	else if (current_state == 2)
	{
		portl_config(0x1E, 0x26, 0x47); //st1_1
	}
	else if (current_state == 3)
	{
		portl_config(0x17, 0x29, 0x43); // st2
	}
	else if (current_state == 4)
	{
		portl_config(0x16, 0x2E, 0x46);//st2_2
	}
	else if (current_state == 5)
	{
		portl_config(0x1D, 0x27, 0x49);//st3
	}
	else if (current_state == 6)
	{
		portl_config(0x1D, 0x27, 0x4E);//st3_3
	}
	time_config(t1, t2, t3);
	mux_time_config(MUX_DELAY);
}

void LightsClass::time_config(uint16_t t1, uint16_t t2, uint16_t t3)
{
	if (current_state == 1)
	{
		state_inc(t1);
	}
	else if (current_state == 3)
	{
		state_inc(t2);
	}
	else if (current_state == 5)
	{
		state_inc(t3);
	}
	else
	{
		state_inc(2);
	}

	if (current_state > 6)
	{
		current_state = 1;
	}
	
}

void LightsClass::state_inc(uint16_t in_tme)
{
	if (millis() - disp_time > in_tme * 1000)
	{
		disp_time = millis();
		current_state++;

	}
}

void LightsClass::mux_time_config(uint32_t dis_delay)
{
	if (micros() - disp_time_mux > dis_delay)
	{
		disp_time_mux = micros();
		current_mux_state++;
		if (current_mux_state > 3)
		{
			current_mux_state = 1;
		}
	}
}