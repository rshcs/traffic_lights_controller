

#define CYCLE_TIME 30
#define SER_RD_TME 20
#define MUX_DLY 3000

uint16_t cd_delay = 1000;
int t1 = 8, t2 = 8, t3 = 8, t4 = 8, t5 = 8, t6 = 8;
int8_t inc = 0, cd_state = 1;
uint32_t cycle_timer = 0, ser_rd_tmr = 0, cd_tmr;

int cd_var = t1;

boolean ser_avl = 0;

// Lights
// port_val[state => 0 1 2 3 4 5 ][point => 0 1 2 (A B C)]
uint8_t portl_val[7][4] = { {0x19, 0x23, 0x47}, 
							{0x1E, 0x26, 0x47}, 
							{0x17, 0x29, 0x43}, 
							{0x16, 0x2E, 0x46}, 
							{0x1D, 0x27, 0x49}, 
							{0x1D, 0x27, 0x4E} };

//Numbers
uint8_t portc_val[11] = { 0x6F, 0x48, 0x3E, 0x5E, 0x59, 0x57, 0x77, 0x4C, 0x7F, 0x5F };

//Seven segment ctrl transistors
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

	cycle_timer = millis();
	ser_rd_tmr = millis();
	
}

void loop()
{
	
	inReader();
	cycle_update(CYCLE_TIME);
	//seg.display_on(cd_var, cd_var, cd_var);
	//lights.state_config(t1, t2, t3);

	lights_test(3000);

	/*
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
	*/
	//count_down_tmr();
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

void display()
{

}

void ports_config(uint8_t pl_val, uint8_t pc_val, uint8_t pa_val)
{
	PORTL = pl_val;
	PORTC = pc_val;
	PORTA = pa_val;
}

void lights_test(uint16_t inDly)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			PORTL = portl_val[i][j];
			//Serial.println(portl_val[i][j], HEX);
			delay(inDly);
		}
	}
}