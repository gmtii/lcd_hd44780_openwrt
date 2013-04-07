/* Display para OpenWrt con PIC18f14k50
Recepción basada en interrupciones y un buffer circular de 64 bytes.

#include <p18f14k50.h> 
#include "lcd.h"
#include "serial.h"
#include <delays.h>


const char quees[] = "Radio WiFi!";
const char iniciando[] = "Iniciando...";


unsigned char buffer[64];
unsigned char i=0,j=0;
unsigned char recibido=0;
unsigned char empezamos=0;
unsigned char vuelta=0;
unsigned char preparado=0;
unsigned long contador=0;
unsigned char temp[16];


	#pragma config CPUDIV = NOCLKDIV 
	#pragma config USBDIV = OFF
	#pragma config FOSC = HS
	#pragma config FCMEN = OFF
	#pragma config IESO = OFF
	#pragma config PLLEN = ON
	#pragma config PCLKEN = OFF
	#pragma config PWRTEN = ON
	#pragma config BOREN = OFF                                                              
	#pragma config WDTEN = OFF
	#pragma config MCLRE = OFF
	#pragma config HFOFST = OFF
	#pragma config STVREN = OFF
	#pragma config LVP = OFF
	#pragma config BBSIZ = OFF
	#pragma config XINST = OFF
	#pragma config CP0 = OFF
	#pragma config CP1 = OFF
	#pragma config CPB = OFF
	#pragma config CPD = OFF
	#pragma config WRT0 = OFF
	#pragma config WRT1 = OFF
	#pragma config WRTB = OFF  
	#pragma config  WRTC = OFF
	#pragma config WRTD = OFF
	#pragma config EBTR0 = OFF
	#pragma config EBTR1 = OFF
	#pragma config EBTRB = OFF

/** P R O T O T Y P E S ****************************************/
void low_isr(void);
void high_isr(void);
void init_buffer(void);

/** V E C T O R S  ************************************************************/
#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
    _asm GOTO high_isr _endasm
}

#pragma  code low_vector=0x18
void interrupt_at_low_vector(void)
{
    _asm GOTO low_isr _endasm
} 

/**************************************************************/
/**************************************************************/
/**************************************************************/


void main() 
 { 

	init_comms();
	LCD_Init();	
	LCD_PrintStr(quees);
	LCD_WriteByte(COMMAND,0xC0);
	LCD_PrintStr(iniciando);
	
	init_buffer();

	while(1) {

			while ( i > j || vuelta ) {

					
					if (buffer[j] == 0x15) 
							LCD_WriteByte(COMMAND, 0x80);
						else if (buffer[j] == 0x16)
					 		LCD_WriteByte(COMMAND, 0xC0);
						else if (buffer[j] == 0x1B) 
					 		LCD_WriteByte(COMMAND, 0x01);
						else if (buffer[j] == 0x13) 
					 		LCD_WriteByte(DATA, ' ');
						else if (buffer[j] == 0x1A) 
					 		preparado=1;
						else if (preparado)
							LCD_WriteByte(DATA, buffer[j]);

						j++;
						if (j>63) { j=0; vuelta=0; }

			}	
			test_oerr();	

	}

}

// Función de interrupción USART

#pragma interrupt high_isr 
void high_isr(void)
{

		 if (PIR1bits.RCIF)   
		 { 
			buffer[i]=RCREG;
			i++;
			if (i>63) { i=0; vuelta=1; }
		
		}

}

// Bucle sin fin

#pragma interruptlow low_isr
void low_isr(void)
{
    while(1)    {;} 
} 

// Inicializa el buffer a espacios.

void init_buffer(void) {

	unsigned int t;

			for (t=0;t<64;t++)
				buffer[t]=0x20;
}
