// ---------------
// Tiedosto: i2c
// Kuvaus: i2c v‰yl‰n ohjaus

#include "i2c.h"

// 1/2 SCL cycle
#define HALF_CYCLE Hus_Delay(2)

#define SDA PORTFbits.RF0
#define SCL	PORTFbits.RF1

int extract(char data, int i) 
{
	return (data >> i) & 0x01;
}

//i2c-v‰yl‰n standardi START-k‰sky
void Start(void)
{
	TRISFbits.TRISF0 = 0x00;
	SCL = 0x01;

	Hus_Delay(3);
	SDA = 0x00;
	Hus_Delay(1);
	SCL = 0x00;
	HALF_CYCLE;
}

//i2c-v‰yl‰n standardi STOP-k‰sky
void Stop(void)
{
	TRISFbits.TRISF0 = 0x00;
	HALF_CYCLE;
	SCL = 0x01;
	HALF_CYCLE;
	SDA = 0x01;
	Hus_Delay(4);
}


//L‰hett‰‰ tavun wr_byte i2c-v‰yl‰‰ pitkin
//Paluuarvo 1, mik‰li vastaanottava laite
//l‰hett‰‰ nack:in ja 0 mik‰li ack:in.
//Send_byten j‰lkeen SCL nollassa
int Send_Byte(unsigned char wr_byte)
{
	int i;
	TRISFbits.TRISF0 = 0x00;
	
	//Luuppi datan lukua varten
	for (i=0; i<8; ++i)
	{
		SDA = extract(wr_byte, (7-i));
		HALF_CYCLE;	
		SCL = 0x01;	
		
		
		HALF_CYCLE;
		SCL = 0x00;
	
	}

	//Asetetaan luku ja luetaan ack tai nack
	TRISFbits.TRISF0 = 0x01;
	HALF_CYCLE;
	SCL = 0x01;

	HALF_CYCLE;

	

	if (SDA == 0)
	{
		SCL = 0x00;
			HALF_CYCLE;
		return 0;
	}
	if (SDA == 1)
	{
		SCL = 0x00;
				HALF_CYCLE;
		return 1;
	}

}

//Lukee v‰yl‰lt‰ tavun ja l‰hett‰‰ sen paluuarvonaan.
//char ack m‰‰ritt‰‰ l‰hetet‰‰nkˆ ack ('a') vai nack ('n')
//Read_byten j‰lkeen SCL nollassa
unsigned char Read_Byte(char ack)
{
	int i;
	unsigned char data;

	TRISFbits.TRISF0 = 0x01;
	SCL = 0x01;
	HALF_CYCLE;	

	//lueataan luupissa data
	for (i=0; i<8; ++i)
	{
		data |= SDA << (7-i);
		HALF_CYCLE;	
		SCL = 0x00;
		
		HALF_CYCLE;
		SCL = 0x01;
	}

	//Asetetaan kirjoitus ja kirjoitetaan ack tai nack
	TRISFbits.TRISF0 = 0x00;
	
	if (ack == 'a')
	{
		SDA = 0x00;
	}
	
	if (ack == 'n')
	{
		SDA = 0x01;
	}

	HALF_CYCLE;
	SCL = 0x00;
	HALF_CYCLE;
	return data;
}

//Noin sadan mikrosekunnin viive. 
//Parametri m‰‰ritt‰‰ viiveen pituuden sadoissa mikrosekunneissa.
//Toteutettu assemblyll‰ erilliseen tiedostoon. 
extern void Hus_Delay (unsigned char);


//Alustaa i2c-v‰yl‰n pinnien (SCL ja SDA) suunnat oikeiksi
//ja asettaa pinnit idle-tilaan (ylˆs)
void Init_i2c(void)
{
	TRISFbits.TRISF1 = 0x00;
	TRISFbits.TRISF0 = 0x00;
	SDA = 0x01;
	SCL = 0x01;
}


//Ms_Viive(int time)
//Purpose: Delay of one millisecond

//Parameters: int time - length of delay in milliseconds
//Return value: - 
void Ms_Viive (int time)
{
  int i;
  int k;	
  for (i = 0; i < time; i++) {
	 for (k = 0; k < VIIVEPARAMETRI; k++);
  }
}



	