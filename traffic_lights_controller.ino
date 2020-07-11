#include "lights.h"
#include "segments.h"

#define CYCLE_TIME 30
#define SER_RD_TME 20

uint16_t cd_delay = 1000;
int t1 = 8, t2 = 8, t3 = 8, t4 = 8, t5 = 8, t6 = 8;
int8_t inc = 0, cd_state = 1;
uint32_t cycle_timer = 0, ser_rd_tmr = 0, mux_tmr = 0, cd_tmr;

int cd_var = t1;

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
	//seg.display_on(cd_var, cd_var, cd_var);
	lights.state_config(t1, t2, t3);
	if (cd_state == 1)
	{
		seg.display_on(cd_var, cd_var, cd_var + t2);
	}
	else if (cd_state == 2)
	{
		seg.display_on(cd_var, cd_var, cd_var);
	}
	else if (cd_state == 3)
	{
		seg.display_on(cd_var + t1, cd_var + t1, cd_var);
	}

	count_down_tmr();
}

void inReader()
{
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

uint16_t count_down_tmr()
{
	if (millis() - cd_tmr > cd_delay)
	{
		cd_tmr = millis();
		cd_var--;
		//Serial.println(cd_var);
		//Serial.println(cd_tmr);
		cd_delay = 1000;
	}
	if (cd_var < 1 )
	{
		if (cd_state == 1)
		{
			cd_var = t2 ;
			cd_state = 2;
			cd_delay = 3000;
			
		}
		else if (cd_state == 2)
		{
			cd_var = t3 ;
			cd_state = 2;
			cd_delay = 3000;
			
		}
		else if (cd_state == 3)
		{
			cd_var = t1 ;
			cd_state = 1;
			cd_delay = 3000;
			
		}	
	}
}

