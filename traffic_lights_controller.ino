
#define MUX_DLY 3000
#define CD_DLY 1000
#define YELLOW_DLY 1

int t1 = 3, t2 = 4, t3 = 5, t4 = 8, t5 = 8, t6 = 8;
int8_t cd_state = 0, location = 0, mux_state;
uint32_t cd_tmr, mux_tmr = 0;

int cd_var = t1;
int cd_var_last;

boolean num_place = 0;

// Lights
// port_val[state => 0 1 2 3 4 5 ][point => 0 1 2 (A B C)]
uint8_t portl_val[7][4] = { {0x19, 0x23, 0x47}, 
							{0x1E, 0x26, 0x47}, 
							{0x17, 0x29, 0x43}, 
							{0x16, 0x2E, 0x46}, 
							{0x1D, 0x27, 0x49}, 
							{0x1D, 0x27, 0x4E} };

//Numbers 0 - 9
uint8_t portc_val[11] = { 0x10, 0xB7, 0xC1, 0x21, 0x26, 0x28, 0x8, 0xB3, 0x00, 0x20 };

//Seven segment ctrl transistors
//A-10, A-01 | B-10, B-01 | C-10, C-01
uint8_t porta_val[7] = {4, 2, 32, 16, 1, 8};

void setup()
{
	Serial.begin(9600);

	DDRL = 0xFF;
	DDRC = 0xFF;
	DDRA = 0xFF;

	PORTL = 0b00001111;
	PORTC = 0xFF;
	PORTA = 0;
	
}

void loop()
{
	//display(t1, t2, t3, 0); // only show received ti t2 t3 values, disable display all 

	display_all(); // show count down timer and leds
	ser_reader(); // read serial received values
	
}

void ser_reader()
{
	if (Serial.available() > 5)
	{
		t4 = Serial.readStringUntil('\n').toInt(); // read until NL character and convert it to integer
		t5 = Serial.readStringUntil('\n').toInt();
		t6 = Serial.readStringUntil('\n').toInt();
		
		while (Serial.available()) // clear excessive data received 
		{
			Serial.read();
		}
	}
}

uint16_t count_down_tmr()
{
	if (millis() - cd_tmr > CD_DLY)
	{
		cd_tmr = millis();
		cd_var--;
	
		//cd_delay = CD_DLY;
	}
	
}

void cd_state_config()
{
	if (cd_var < 1)
	{
		if (cd_state == 0)
		{
			cd_var_last = cd_var;
			cd_state = 1;
			cd_var = YELLOW_DLY;
		}
		else if (cd_state == 1)
		{
			cd_var = t2;
			cd_state = 2;

		}
		else if (cd_state == 2)
		{
			cd_var_last = cd_var;
			cd_state = 3;
			cd_var = YELLOW_DLY;


		}
		else if (cd_state == 3)
		{
			cd_var = t3;
			cd_state = 4;

		}
		else if (cd_state == 4)
		{
			cd_var_last = cd_var;
			cd_state = 5;
			cd_var = YELLOW_DLY;

		}
		else if (cd_state == 5)
		{
			cd_var = t1;
			cd_state = 0;
		}
	}
}

void lights_test(uint16_t inDly)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			PORTL = portl_val[i][j];
			delay(inDly);
		}
	}
}

void segments_test(uint16_t inDly)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			PORTC = portc_val[j];
			PORTA = porta_val[i];
			
			delay(inDly);
		}
	}
}

uint8_t seg_out(uint8_t in_val, boolean place)
{
	if (in_val > 99)
	{
		PORTC = portc_val[9];
	}
	else if(!place)
	{
		PORTC = portc_val[in_val % 10];
	}
	else if (place)
	{
		PORTC = portc_val[in_val / 10];
	}
}

void display(int ta, int tb, int tc, int8_t state_in)
{
	if (micros() - mux_tmr > MUX_DLY)
	{
		mux_tmr = micros();
		
		if (location == 0)
		{
			seg_out(ta, num_place);
			PORTL = portl_val[state_in][location];
			
		}
		else if (location == 1)
		{
			seg_out(tb, num_place);
			PORTL = portl_val[state_in][location];
			
		}
		else if(location == 2)
		{
			seg_out(tc, num_place);
			PORTL = portl_val[state_in][location];
	
		}

		if (num_place)
		{
			PORTA = porta_val[location];
			
		}
		else
		{
			PORTA = porta_val[location + 3];
			
		}

		location++;
		if (location > 2)
		{
			location = 0;
			num_place = !num_place;
		}
	}
}

void display_all()
{
	if (cd_state == 0)
	{
		display(cd_var, cd_var, cd_var + t2, cd_state);

	}
	else if (cd_state == 1) //<==== transition state
	{
		display(cd_var_last, cd_var_last, cd_var_last + t2, cd_state);
	}
	else if (cd_state == 2)
	{
		display(cd_var, cd_var, cd_var, cd_state);

	}
	else if (cd_state == 3)//<==== transition state
	{
		display(cd_var_last, cd_var_last, cd_var_last, cd_state);
	}
	else if (cd_state == 4)
	{
		display(cd_var + t1, cd_var + t1, cd_var, cd_state);

	}
	else if (cd_state == 5)//<==== transition state
	{
		display(cd_var_last + t1, cd_var_last + t1, cd_var_last, cd_state);
		
		if (t1 != t4 || t2 != t5 || t3 != t6) // only update values if they changed
		{
			t1 = t4;
			t2 = t5;
			t3 = t6;
		}
	}

	count_down_tmr(); 
	cd_state_config(); // count down state
}