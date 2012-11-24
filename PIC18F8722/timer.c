// ---------------
// Tiedosto: timer.c
// Kuvaus: Vilkuttaa ledia timer1 ajastimen compare macth keskeytysten tahtiin
// Luonti-pvm: 26/09/2011
// Viimeksi muokattu: 
// Korjaukset: -
//
// --------------- 

#include <p18f8722.h>

// Comment out if you don't use conf_bits.h file
// #include "conf_bits.h"
void timer_routine (void);
void delay (void);

//Conf bits:
#pragma config OSC = INTIO67    // INTRC-OSC2 as RA6, OSC1 as RA7
#pragma config LVP = OFF        // Low Voltage ICSP
#pragma config WDT = ON        // Watchdog Timer (enable with SW)
#pragma config WDTPS = 32768    // WDT Postscaler: 1024*4ms = ~4.1 sec
#pragma config DEBUG = ON      // Background Debugger

//Oskillaattorin, Timerin ja 
//keskeytysten alustus 
#define INTSRC OSCTUNEbits.INTSRC

#define T1CKPS1 T1CONbits.T1CKPS1
#define T1CKPS0 T1CONbits.T1CKPS0
#define T1OSCEN T1CONbits.T1OSCEN
#define TMR1CS T1CONbits.TMR1CS
#define T1SYNC T1CONbits.T1SYNC
#define CCP1IF PIR1bits.CCP1IF
#define CCP1IE PIE1bits.CCP1IE
#define TMR1ON T1CONbits.TMR1ON

#define IPEN RCONbits.IPEN
#define PEIE INTCONbits.PEIE
#define GIE INTCONbits.GIE

//LED 
#define LED_DIR TRISGbits.TRISG3
#define LED_PWR PORTGbits.RG3 

//nappi
#define BUTTON_DIR TRISBbits.PORTB0
#define BUTTON PORTBbits.RB0

#define INT0IF INTCONbits.INT0IF
#define INT0IE INTCONbits.INT0IE
#define RBPU INTCON2bits.RBPU
#define INTEDG0 INTCON2bits.INTEDG0

//Muuttuja ledin tilalle
unsigned int state;

unsigned int speedmode;

//Keskeytysrutiinin esittely ja
//keskeytysvektorin luonti
#pragma code timer_interrupt = 0x8
void timer_int (void)
{
  _asm
    goto timer_routine
  _endasm
}
#pragma code

//Keskeytystenkasittelija
#pragma interrupt timer_routine 
void timer_routine (void)
{
  if (CCP1IF)
  {
	CCP1IF = 0;

	_asm
		clrwdt
	_endasm
	
	if (state == 0)
	{
		LED_PWR = 1;
		state = 1;
	}
	else
	{
		LED_PWR = 0;
		state = 0;
	}
  }

  if (INT0IF)
  {
	delay();
	if (speedmode == 0)
	{
 		CCPR1H = 0x40;
		INT0IF = 0;
		speedmode = 1;
	}
	else
	{
		CCPR1H = 0x80;
		INT0IF = 0;
		speedmode = 0;
	}
  }
}

//Viive varahtelyn poistoon
void delay (void)
{
  	int i;

  	for (i = 0; i < 1000; i++);	 
}

void main (void)
{
  //Asetetaan oikea alkuarvo 
  //oskillaattorille(31kHz)
  INTSRC = 0;
	
  //Alustetaan Timer1
  T1CKPS1 = 0;
  T1CKPS0 = 0;
  T1OSCEN = 1;
  TMR1CS = 1;
  T1SYNC = 0;
  
  TRISC = 0;
  TRISB = 0xEF;
  INTEDG0 = 0;
  
  CCPR1H = 0x80;
  CCPR1L = 0x00;
  CCP1CON = 0x0B;

  CCP1IF = 0;
  CCP1IE = 1;

  RBPU = 0;
  INT0IF = 0;
  INT0IE = 1;

  TMR1ON = 1;
  IPEN = 0;
  PEIE = 1;
  GIE = 1;

  //Ledin alustukset
  LED_DIR = 0;
  LED_PWR=0;

  state = 0;
  speedmode = 0;

  while (1)
  {
	//Looping :)	
  }
}
