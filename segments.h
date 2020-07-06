// segments.h

#ifndef _SEGMENTS_h
#define _SEGMENTS_h

#define SEG_DELAY 3000

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SegmentsClass
{
 private:


	 //PORTA values according to segment control transistors
	 int8_t seg_a[3] = { 4, 16 };
	 int8_t seg_b[3] = { 2, 1 };
	 int8_t seg_c[3] = { 32, 8 };

	 // 0 to 10
	 uint8_t nums[11] = { 0x6F, 0x48, 0x3E, 0x5E, 0x59, 0x57, 0x77, 0x4C, 0x7F, 0x5F };
	 uint32_t disp_tmr = 0;
	 boolean seg_select = 0;
 public:
	 SegmentsClass();
	 void init();

	 void segment_out(uint8_t inVal);
	 void segment_ctrl(uint8_t inVal);
	 void test_segments(uint16_t delay_t);
	 int16_t powr(int16_t inNum, int16_t inPow);
	 //void segment_b(uint8_t in_val);
	 void segment_on(int8_t* in_addr, int in_val);
	 void display_on(int t1, int t2, int t3);
	 void disp_tmr_set();
	 void disp_tmr_run(uint32_t t1);
};

//extern SegmentsClass Segments;

#endif

