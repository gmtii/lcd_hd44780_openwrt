#include <p18f14k50.h> 
#include "serial.h"
#include <usart.h>

/* Ajustar el valor de divisor según la siguiente fórmula:

 FOSC/[4 (n + 1)] 
 SPBRGH:SPBRG=(FOSC/baudrate)/4-1

donde FOSC en la frecuencia de cristal en Hz. 

*/


#define divisor 86


// Inicia la configuración de USART

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

// Manda carácter

void putch(unsigned char byte) 
{

	while(!TXSTAbits.TRMT)	/* bucle esperando TRMT */
		continue;
	TXREG = byte;
}

// Recibe carácter

unsigned char getch() {
	/* retrieve one byte */
	while(!PIR1bits.RCIF)	/* bucle esperando RCIF */
		continue;
	return RCREG;	
}

// Recibe carácter con eco

unsigned char getche(void)
{
	unsigned char c;
	putch(c = getch());
	return c;
}

// Envía string

void txstring(const char *s , unsigned char fl)
{
    while(*s)
        putch(*s++);

	if (fl) {
	putch('\r');
	putch('\n'); }
}

// Detecta buffer overrun, reiniciando el USART a continuación.

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
