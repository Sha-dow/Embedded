// ---------------
// Tiedosto: usart.c
// Kuvaus: USART-väylän testikoodi, kaiutetaan merkit näytölle, 
// 		   merkillä 'g' sytytetään ledi ja merkillä 'h' sammutetaan.
// Luonti-pvm: 14/09/2011
// Viimeksi muokattu: 24/09/2011
// Korjaukset: 
//
// --------------- 

//Include-direktiivit c-kirjastoille ja 
//laitteistokohtaisille kirjastoille
#include <p18f8722.h>
#include <usart.h>

#include <stdio.h>
#include <ctype.h>

//Konfiguraatiobitit
#include "conf_bits.h"

//LED portit
#define LED_DIR TRISGbits.TRISG3
#define LED_PWR PORTGbits.RG3 

//Oskillaattorin alustus 
//sisaiseen 4Mhz modeen
#define IRCF2 OSCCONbits.IRCF2
#define IRCF1 OSCCONbits.IRCF1
#define IRCF0 OSCCONbits.IRCF0

//Funktioiden esittelyt
void rx_routine (void);
void Open1USART(unsigned char config,
			   unsigned int spbrg);
void puts1USART(char *data);
char Read1USART(void);

//Keskeytysrutiinin esittely ja
//keskeytysvektorin luonti
#pragma code rx_interrupt = 0x8
void rx_int (void)
{
	_asm
		goto rx_routine
	_endasm
}
#pragma code

//Keskeytystenkasittelija
#pragma interrupt rx_routine 
void rx_routine (void)
{
	//Alustetaan muuttuja
	//charracterille
	unsigned char character;

	//Luetaan merkki UART-
	//vaylasta
	character = Read1USART();

	//Jos merkki oli g, sytytetaan
	//ledi ja kaiutetaan.
	//Jos merkki oli h, sammutetaan
	//ledi ja kaiutetaan. 
	//Muissa tapauksissa vain 
	//kaiutetaan
	if (character == 'g')
	{
		LED_PWR = 1;
		printf("%c",character);
	}
	else if (character == 'h')
	{
		LED_PWR = 0;
		printf("%c",character);
	}
	else
	{
		printf("%c",character);
	}
	
	//Nollataan keskeytysbitti
	PIR1bits.RCIF = 0;
}

void main (void)
{
	//Asetetaan oikeat alkuarvot 
	//oskillaattorin biteille...
	IRCF2 = 1;
	IRCF1 = 1;
	IRCF0 = 0;

	//...ja ledille
	LED_DIR = 0;
 	LED_PWR=0;	
	TRISB = 0;

	//Avataan USART-vayla
	//8-1 bit, 9600baud, asych
	Open1USART(USART_TX_INT_OFF &
			  USART_RX_INT_ON &
			  USART_ASYNCH_MODE &
			  USART_EIGHT_BIT &
			  USART_CONT_RX &
			  USART_BRGH_HIGH, 25);

	//Sallitaan keskeytykset
	RCONbits.IPEN =1;

	//Vastaanotetuista keskeytyksista
	//korkean prioriteetin keskeytyksia
	IPR1bits.RCIP = 1;

	//Sallitaan kaikki korkean 
	//prioriteetin keskeytykset
	INTCONbits.GIEH = 1;

	//Loopataan ikuisesti ja 
	//odotetaan keskeytyksia
	while (1);

}