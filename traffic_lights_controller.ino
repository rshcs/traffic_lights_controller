#include "lights.h"
#include "segments.h"

#define CYCLE_TIME 10

int t1 = 10, t2 = 0, t3 = 0;
int8_t inc = 1;
uint32_t cycle_timer = 0;

LightsClass lights(0);
SegmentsClass seg;

void setup()
{
	Serial.begin(9600);
	cycle_timer = millis();
}

void loop()
{
	inReader();
	seg.display_on(t1, t2, t3);
}

void inReader()
{
	if (Serial.available())
	{
		delay(20);
		if (inc == 1)
		{
			t1 = inStringReader().toInt();
			inc++;
		}
		else if (inc == 2)
		{
			t2 = inStringReader().toInt();
			inc++;
		}
		else if (inc == 3)
		{
			t3 = inStringReader().toInt();
			inc = 1;
		}
	}
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

