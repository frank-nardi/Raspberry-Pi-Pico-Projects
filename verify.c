#include "systick.h"
#include "led.h"
#include "usbcdc.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdint.h>

const char code[] = "12345";
const int len = 5;
void task( char input );

int main( void )
{
	__disable_irq();

	configure_usbcdc();
	configure_led();

	__enable_irq();

	char character;
	while(1)
	{
		if( usbcdc_getchar(&character) )
			task(character);
	}
	return 0;
}

void task( char input )
{
	static uint8_t index;
	if( index != len )
	{
		turn_off_led();
		if( input == code[index] )
		{
			usbcdc_putchar(input);
			index++;
		}
		else
		{
			index = 0;
			usbcdc_putchar('X');
			usbcdc_putchar('\n');
			usbcdc_putchar('\r');
		}
	}
	if( index == len ){
		index = 0;
		turn_on_led();
		usbcdc_putchar('\n');
		usbcdc_putchar('\r');
		usbcdc_putchar('O');
		usbcdc_putchar('\n');
		usbcdc_putchar('\r');
	}
}