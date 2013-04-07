#include <p18f14k50.h> 
#include "serial.h"
#include <usart.h>

/* Ajustar el valor de divisor seg�n la siguiente f�rmula:

 FOSC/[4 (n + 1)] 
 SPBRGH:SPBRG=(FOSC/baudrate)/4-1

donde FOSC en la frecuencia de cristal en Hz. multiplicada por 4
que es el multiplicador PLL configurado en los conf bits.

*/


#define divisor 86


// Inicia la configuraci�n de USART

void init_comms(void)
{
    TRISBbits.TRISB5=1; 	// Pin UART RX
    TRISBbits.TRISB7=0; 	// Pin UART TX

    ANSEL = 0;
    ANSELH = 0; 

    //SPBRGH  = 0;
    //SPBRG   = 86;

	OpenUSART(  USART_TX_INT_OFF & 
				USART_RX_INT_ON & 
				USART_ASYNCH_MODE & 
				USART_EIGHT_BIT & 
				USART_CONT_RX & 
				USART_BRGH_HIGH, divisor );  

    BAUDCONbits.BRG16=1;	

	RCONbits.IPEN = 1;
	PIE1bits.RCIE = 1;
	IPR1bits.RCIP=1;
	INTCONbits.GIEH = 1; 

}

// Manda car�cter

void putch(unsigned char byte) 
{

	while(!TXSTAbits.TRMT)	/* bucle esperando TRMT */
		continue;
	TXREG = byte;
}

// Recibe car�cter

unsigned char getch() {
	/* retrieve one byte */
	while(!PIR1bits.RCIF)	/* bucle esperando RCIF */
		continue;
	return RCREG;	
}

// Recibe car�cter con eco

unsigned char getche(void)
{
	unsigned char c;
	putch(c = getch());
	return c;
}

// Env�a string

void txstring(const char *s , unsigned char fl)
{
    while(*s)
        putch(*s++);

	if (fl) {
	putch('\r');
	putch('\n'); }
}

// Detecta buffer overrun, reiniciando el USART a continuaci�n.

void test_oerr(void) {

	unsigned char temp;

	if (RCSTAbits.OERR)
        {
	   temp=RCREG;
	   temp=RCREG;
	   RCSTAbits.CREN = 0;     
	   RCSTAbits.CREN = 1;
		}
}
