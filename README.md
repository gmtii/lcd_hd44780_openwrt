Interface serial TTL->PIC->HD44780 para incorporar una pantalla a un
router OpenWrt.

Este ejemplo está hecho para un display de 16x2.

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
