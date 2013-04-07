Interface serial TTL->PIC->HD44780 para incorporar una pantalla a un
router OpenWrt.

Este ejemplo está hecho para un display de 16x2 y el script a ejecutar en 
el router de ejemplo es lcd.sh

Inicia mostrando dos mensajes fijos mientras el router va arrancando.

Hasta que un script no envíe el carácter 0x1A por la consola /dev/ttyS0
no se activa la representación en pantalla.

Una activada, debe anteponerse un carácter especial antes de lo que 
queramos representar para señalar la línea del display que queremos
utilizar, o borrar la pantalla por ejemlo.

Estos caracteres especiales para realizar funciones con el display
vienen datos según el siguiente else if:

                                        
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

0x15 = seleciona linea 0 del display
0x16 = seleciona linea 1 del display
0x1B = clear
0x13 = interpreta un return como un espacio (evita saltos en log de arranque)
0x1A = pone preparado a 1 e inicia la representación en el display

Pueden incoporarse más líneas o comandos según nuestro display.

Configuración:

Compilar con MPLAB y ajustar la velocidad de puerto que queremos usar en el 
serial.c según al fórmula indicada. Tal cual está configurada para 10 Mhz con 
115200 bps= (Velocidad efetiva es la frecuencia dle cristobal x 4 PLL):

p.e.: SPBRGH:SPBRG=(FOSC/baudrate)/4-1 para un cristal de 10 Mhz da:

10.000.000x4/115200-1=86 que es el valor por defecto.

El conexionada de la pantalla es como sigue:

/* Display tipo HD44780 conectado al puerto C del PIC 
según la siguiente correspondencia:

Pin 6, RC4 -> Display D4
Pin 5, RC5 -> Display D5
Pin 8, RC6 -> Display D6
Pin 9, RC7 -> Display D7

Pin 15, RC1 -> Display LCD_RS
Pin 14, RC2 -> Display LCD_RW (también puede ir a GND)
Pin 7 , RC3 -> Display LCD_EN
*/

