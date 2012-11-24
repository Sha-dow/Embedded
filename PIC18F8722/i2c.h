//i2c.h

#ifndef I2C_H
#define I2C_H

#include <p18f8722.h>


#define SDA PORTFbits.RF0
#define SCL PORTFbits.RF1

#define SDA_direction TRISFbits.TRISF0
#define SCL_direction TRISFbits.TRISF1

//Prosessorin kellotaajuus
#define KELLOTAAJUUS 4000000
#define VIIVEPARAMETRI (KELLOTAAJUUS/4000000)*35


//i2c-v‰yl‰n standardi START-k‰sky
void Start(void);

//i2c-v‰yl‰n standardi STOP-k‰sky
void Stop(void);


//L‰hett‰‰ tavun wr_byte i2c-v‰yl‰‰ pitkin
//Paluuarvo 1, mik‰li vastaanottava laite
//l‰hett‰‰ nack:in ja 0 mik‰li ack:in.
//Send_byten j‰lkeen SCL nollassa
int Send_Byte(unsigned char wr_byte);

//Lukee v‰yl‰lt‰ tavun ja l‰hett‰‰ sen paluuarvonaan.
//char ack m‰‰ritt‰‰ l‰hetet‰‰nkˆ ack ('a') vai nack ('n')
//Read_byten j‰lkeen SCL nollassa
unsigned char Read_Byte(char ack);

//time * noin millisekunnin viive
void Ms_Viive (int time);

//Noin sadan mikrosekunnin viive. 
//Parametri m‰‰ritt‰‰ viiveen pituuden sadoissa mikrosekunneissa. 
extern void Hus_Delay (unsigned char);

//Alustaa i2c-v‰yl‰n pinnien (SCL ja SDA) suunnat oikeiksi
//ja asettaa pinnit idle-tilaan (ylˆs)
void Init_i2c(void);

int extract(char data, int i);

#endif