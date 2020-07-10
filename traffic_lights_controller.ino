#include "lights.h"
#include "segments.h"

#define CYCLE_TIME 30
#define SER_RD_TME 20
#define MUX_DELAY 3000

int t1 = 10, t2 = 0, t3 = 0, t4, t5, t6;
int8_t inc = 0;
uint32_t cycle_timer = 0, ser_rd_tmr = 0, mux_tmr = 0;

LightsClass lights(0);
SegmentsClass seg;
boolean ser_avl = 0;

void setup()
{
	Serial.begin(9600);
	cycle_timer = millis();
	ser_rd_tmr = millis();
	mux_tmr = micros();
}

void loop()
{
	inReader();
	cycle_update(CYCLE_TIME);
	//seg.display_on(t1, t2, t3);
	//lights.state_config(4, 6, 10);
}

void inReader()
{
	/*
	if (Serial.available())
	{
		delay(20);
		if (inc == 1)
		{
			t1 = inStringReader().toInt();
			
			//inc++;
		}
		else if (inc == 2)
		{
			t2 = inStringReader().toInt();
			//inc++;
		}
		else if (inc == 3)
		{
			t3 = inStringReader().toInt();
			//inc = 1;
		}
	}
	*/
	
	if (inc == 1)
	{
		t4 = inStringReader().toInt();
		inc++;
	}
	else if (inc == 3)
	{
		t5 = inStringReader().toInt();
		inc++;
	}
	else if (inc == 5)
	{
		t6 = inStringReader().toInt();
		inc = 0;
	}
	read_delay(20);
	
}

String inStringReader()
{
	String inStr = "";
	while (Serial.available() > 0)
	{
		inStr += Serial.readString();
	}
	return inStr;
}

void read_delay(uint16_t delay_tme)
{
	if (Serial.available() && !ser_avl)
	{
		ser_rd_tmr = millis();
		ser_avl = 1;
		
	}
	if (ser_avl && (millis() - ser_rd_tmr > delay_tme))
	{
		//Serial.println(inc);
		ser_avl = 0;
		inc++;
		
	}
}

void cycle_update(uint16_t upd_time) // update t1, t2, t3 for every entered value (in seconds)
{
	
	if (millis() - cycle_timer > upd_time * 1000)
	{
		cycle_timer = millis();
		t1 = t4;
		t2 = t5;
		t3 = t6;
		//Serial.println("cyc tme");
	}
}

boolean mux_config()
{
	if (micros() - mux_tmr > MUX_DELAY)
	{
		mux_tmr = micros();
		return 1;
	}
	else
	{
		return 0;
	}
}