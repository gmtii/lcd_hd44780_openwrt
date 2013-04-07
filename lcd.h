/* Display tipo HD44780 conectado al puerto C del PIC 
según la siguiente correspondencia:

Pin 6, RC4 -> Display D4
Pin 5, RC5 -> Display D5
Pin 8, RC6 -> Display D6
Pin 9, RC7 -> Display D7

Pin 15, RC1 -> Display LCD_RS
Pin 14, RC2 -> Display LCD_BL
Pin 7 , RC3 -> Display LCD_EN
*/


#define	COMMAND		0
#define DATA		1
#define LCD_RS		LATCbits.LATC1
#define LCD_BL		LATCbits.LATC3
#define LCD_EN		LATCbits.LATC2


#define LCD_Init_Pin()	LATC=0;TRISC &= 0; 				

void LCD_WriteByte(unsigned char reg, unsigned char data);
void LCD_PrintStr(const char *str);
void LCD_PulseEnable(void);
void LCD_Init(void);
