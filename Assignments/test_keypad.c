#include <stdbool.h>
#include <stdint.h>
#include "systick.h"
#include "keypad.h"
#include "interrupt.h"
#include "usbcdc.h"

void main( void )
{
	char character;
	__disable_irq();

	configure_keypad();
	configure_systick();
	configure_usbcdc();
    systick_register_callback(task_keypad_scan);
	

    __enable_irq();

	while(1){
        __asm__("WFI");
        if(!system_tick())
            continue;   
		if( keypad_getchar(&character) )
			usbcdc_putchar(character);
	}	
}
