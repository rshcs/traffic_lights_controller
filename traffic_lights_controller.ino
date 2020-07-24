
#define MUX_DLY 3000 // multiplexer delay 3 milliseconds
#define CD_DLY 1000 // count down delay 1 second
#define YELLOW_DLY 1 // yellow led stays for 1 second 
#define TIME_ONLY 6 // Parameter use with display_all(TIME_ONLY) 
#define YELLOW_LED_ONLY 7 // Parameter use with display_all(YELLOW_LED_ONLY)
#define ALL_OFF 8 // Parameter use with display_all(ALL_OFF)

int t1 = 3, t2 = 4, t3 = 5, t4 = 8, t5 = 8, t6 = 8;
//------------------
int8_t cd_state = 0; // count down state
//------------------
int8_t location = 0, mux_state;
uint32_t cd_tmr, mux_tmr = 0; // count down timer, multiplexer timer

int cd_var = t1; // count down variable
int cd_var_last; // last count down variable

boolean num_place = 0; // seven segment digit 0 or 1

// Lights
// port_val[state => 0 1 2 3 4 5 ][point => 0 1 2 (A B C)]
uint8_t portl_val[9][4] = { {0x19, 0x23, 0x47}, //State 1	[0][x]
							{0x1E, 0x26, 0x47}, // Transition state 1	[1][x]
							{0x17, 0x29, 0x43}, //State 2	[2][x]
							{0x16, 0x2E, 0x46}, // Transition state 2	[3][x]
							{0x1D, 0x27, 0x49}, //State 3 [4][x]
							{0x1D, 0x27, 0x4E},	// Transition state 3	[5][x]
							{0x0F, 0x0F, 0x0F}, // All off	[6][x]
							{0x1E, 0x2E, 0x4E} }; // Yellow all		[7][x]

//Numbers 0 - 9 and [10] = "No Output" on seven segment
uint8_t portc_val[12] = { 0x10, 0xB7, 0xC1, 0x21, 0x26, 0x28, 0x8, 0xB3, 0x00, 0x20, 0xFF };

//Seven segment ctrl transistors
//A-10, A-01 | B-10, B-01 | C-10, C-01 | All Off
uint8_t porta_val[7] = {4, 2, 32, 16, 1, 8 };

void setup()
{
	Serial.begin(9600);

	DDRL = 0xFF; // All outputs
	DDRC = 0xFF;
	DDRA = 0xFF;

	blink_nx(500, 3);

	PORTL = 0b00001111; // 0-3 bits set 1
	PORTC = 0xFF; // all bits set 1
	PORTA = 0; // all bits set 0
	delay(500);
	cd_tmr = millis();
	mux_tmr = millis();
}

void loop()
{
	display_all(cd_state); 
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

void count_down_tmr()
{
	if (millis() - cd_tmr > CD_DLY)
	{
		cd_tmr = millis();
		cd_var--; // count down variable
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

void lights_test(uint16_t inDly) // Check each LED
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

void segments_test(uint16_t inDly) // Check each segment
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

void seg_out(int in_val, boolean place)
{
	if (in_val > 99)
	{
		PORTC = portc_val[9];
	}
	else if (in_val < 0)
	{
		PORTC = portc_val[10];
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
			seg_out(ta, num_place); // seven segment location A
			PORTL = portl_val[state_in][location]; // LED status location A
			
		}
		else if (location == 1)
		{
			seg_out(tb, num_place); // seven segment location B
			PORTL = portl_val[state_in][location]; // seven segment location B
			
		}
		else if(location == 2)
		{
			seg_out(tc, num_place); // seven segment location C
			PORTL = portl_val[state_in][location]; // seven segment location C
	
		}

		if (num_place) // num_place = 0 or 1
		{
			PORTA = porta_val[location]; // location = Seven segment control transistor output 1 of the six on at a time
		}
		else
		{
			PORTA = porta_val[location + 3]; // Next digit on Seven segment	
		}

		location++;
		if (location > 2)
		{
			location = 0; // Reset
			num_place = !num_place; // Reset Seven segment 
		}
	}
}

void display_all(int8_t in_state)
{
	if (in_state == 0) // <=== State 1
	{
		display(cd_var, cd_var, cd_var + t2, in_state);
	}
	else if (in_state == 1) //<==== transition state
	{ // Yellow light(s) on in this state
		display(cd_var_last, cd_var_last, cd_var_last + t2, in_state);
	}
	else if (in_state == 2) // <=== State 2
	{
		display(cd_var, cd_var, cd_var, in_state);
	}
	else if (in_state == 3)//<==== transition state
	{// Yellow light(s) on in this state
		display(cd_var_last, cd_var_last, cd_var_last, in_state);
	}
	else if (in_state == 4) // <=== State 3
	{
		display(cd_var + t1, cd_var + t1, cd_var, in_state);
	}
	else if (in_state == 5)//<==== transition state
	{// Yellow light(s) on in this state
		display(cd_var_last + t1, cd_var_last + t1, cd_var_last, in_state);

		if (t1 != t4 || t2 != t5 || t3 != t6) // only update values if they changed
		{
			t1 = t4; // Update serial received values
			t2 = t5;
			t3 = t6;
		}
	}
	else if (in_state == 6) // Shows received t1, t2, t3 on Seven segments, No LEDs
	{
		display(t4, t5, t6, in_state);
	}
	else if (in_state == 7) // Yellow LEDs only
	{
		display(-1, -1, -1, in_state);
	}
	else if (in_state == 8) // All off
	{
		display(-1, -1, -1, TIME_ONLY);
	}

	if (in_state < 6)
	{
		count_down_tmr();
		cd_state_config(); // count down state
	}
}

void blink_nx(uint16_t in_dly, uint8_t in_blnk)
{
	boolean disp_on_off = 0;
	uint16_t in_millis = millis();
	for (int8_t i = 1; i < in_blnk * 2; i++)
	{
		while (in_millis + i * in_dly > millis())
		{
			display_all(7 + disp_on_off);
		}
		disp_on_off = !disp_on_off;	
	}
}