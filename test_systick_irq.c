#include <stdint.h>
#include "systick.h"
#include "led.h"
#include "interrupt.h"

void main( void )
{
    __disable_irq();

	configure_led();
	configure_systick();
    systick_register_callback(pulse_led);

    __enable_irq();

	while(1){
        __asm__("WFI");
        if(!system_tick())
            continue;   
	}
}