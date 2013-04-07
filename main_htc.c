#include <p18f14k50.h> 
#include "lcd.h"
#include "serial.h"
#include <delays.h>



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


void main() 
 { 

    ADCON0bits.ADON = 0;    // ADC disabled

	LCD_Init();	
	LCD_PrintStr("Radio Wifi");

	init_comms();
	//txstring("",1);
     
	while(1)
	;
}

void interrupt highISR(void)
{

 if (PIE1bits.RCIE && PIR1bits.RCIF)   // USART receive interrupt
 { 
	unsigned char recibido=RCREG;

	if (RCSTAbits.OERR)
	        {
		LCD_WriteByte(DATA, recibido);
	   recibido = RCREG;
		LCD_WriteByte(DATA, recibido);
	   RCSTAbits.CREN = 0;     
	   RCSTAbits.CREN = 1;
	   return;
	        }

	if (RCSTAbits.FERR)
	  {
		LCD_WriteByte(DATA, 'F');
	   return;
	  } 


    if (recibido < 0x20 ) {
    
        switch (recibido) {

            case 0x1B:
			 LCD_WriteByte(COMMAND, 0x01);
              break;
            case 0x15:
             LCD_WriteByte(COMMAND,0x80);
              break;
            case 0x16:
              LCD_WriteByte(COMMAND,0xC0);
              break;
            case 0x13:
              LCD_WriteByte(COMMAND,0x20);
              break;
        }

    }
    else
	LCD_WriteByte(DATA, recibido);

	PIR1bits.RCIF = 0; 
	}


}