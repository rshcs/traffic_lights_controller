
#include "segments.h"

SegmentsClass::SegmentsClass()
{

}

void SegmentsClass::init()
{
	DDRL = 0xFF; // PORTL => output
	DDRC = 0xFF; // PORTC => output 
	DDRA = 0xFF; // PORTA => output

	PORTL = 0b00001111; // LEDs and LED control transistors
	PORTC = 0x7F; //7Segments
	PORTA = 0; // 7Segment control transistors

}

void SegmentsClass::segment_out(uint8_t inVal)
{
	
	uint8_t port_val = 0x7F & ~inVal;
	PORTC = port_val;
}

void SegmentsClass::segment_ctrl(uint8_t inVal)
{
	
	uint8_t port_val = 0x3F & inVal;
	PORTA = port_val;

}

void SegmentsClass::test_segments(uint16_t delay_t) // Test function
{
	for (int i = 0; i < 7; i++)
	{
		segment_out(powr(2, i));
		for (int i = 0; i < 6; i++)
		{
			segment_ctrl(powr(2, i));
			delay(delay_t);
		}
	}
}

int16_t SegmentsClass::powr(int16_t inNum, int16_t inPow) // this function associates with test function
{

	int16_t outNum = 1;
	for (int16_t i = 0; i < inPow; i++)
	{
		outNum *= inNum;
	}
	return outNum;
}

void SegmentsClass::segment_on(int8_t* in_addr, int in_val)
{
	if (in_val > 99)
	{
		if (seg_select)
		{
			PORTA = 0;
			PORTC = 0x7F;
			segment_out(nums[9]); // number to display
			segment_ctrl(*in_addr); //transistor number to on
			//delay(3);
		}
		else
		{
			PORTA = 0;
			PORTC = 0x7F;
			segment_out(nums[9]);
			segment_ctrl(*(in_addr + 1)); // next transistor is on
			//delay(3);
		}	
	}
	else
	{
		if (seg_select)
		{
			PORTA = 0;
			PORTC = 0x7F;
			segment_out(nums[in_val / 10]);
			segment_ctrl(*in_addr);
			//delay(SEG_DELAY);
		}
		else
		{
			PORTA = 0;
			PORTC = 0x7F;
			segment_out(nums[in_val % 10]);
			segment_ctrl(*(in_addr + 1));
			//delay(SEG_DELAY);
		}
	}
	disp_tmr_run(SEG_DELAY);
}

void SegmentsClass::display_on(int t1, int t2, int t3)
{
	segment_on(&seg_a[0], t1);
	segment_on(&seg_b[0], t2);
	segment_on(&seg_c[0], t3);
}

void SegmentsClass::disp_tmr_set() // Equate disp_tmr to micros
{
	disp_tmr = micros();
}

void SegmentsClass::disp_tmr_run(uint32_t t1)
{
	if (micros() - disp_tmr > t1)
	{
		
		seg_select = !seg_select;
		disp_tmr = micros();
	}
}