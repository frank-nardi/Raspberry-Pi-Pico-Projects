#include "servo_pin0.h"
#include "usbcdc.h"
#include "interrupt.h"
#include <stdio.h>
#include <stdint.h>

#define STEP_SIZE 100

static uint16_t current_pos;
void task( char input );

int main( void )
{
	__disable_irq();

	configure_servo_pin0();
	configure_usbcdc();

	__enable_irq();

	current_pos = 0;
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
	usbcdc_putchar(input);

	if( input == 'l'){
		current_pos += STEP_SIZE;
	}

	if( input == 'L'){
		current_pos += (2*STEP_SIZE);
	}

	if( input == 'r'){
		current_pos -= STEP_SIZE;
	}

	if( input == 'R'){
		current_pos -= (2*STEP_SIZE);
	}

	if( input == 'c'){
		current_pos = 0;
	}
	rotate_servo_pin0(current_pos);
}