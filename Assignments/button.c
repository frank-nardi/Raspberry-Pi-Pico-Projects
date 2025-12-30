#include "button.h"
#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include <stdbool.h>
#include <stdint.h>

#define BTN_LOC 7
#define CONCAT2(X,Y) X ## Y
#define CONCAT3(X,Y,Z) X ## Y ## Z
#define IO_BANK0_GPIO_CTRL(X) CONCAT3(IO_BANK0_GPIO,X,_CTRL)
#define PADS_BANK0_GPIO(X) CONCAT2(PADS_BANK0_GPIO,X)
#define GPIO_RESETS (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK)

#define HALF_SECOND 100
#define FULL_SECOND 200

void configure_button( void )
{
    resets.reset_clr = GPIO_RESETS;
    while(! (resets.reset_done & GPIO_RESETS) )
	    continue;

    PADS_BANK0_GPIO( BTN_LOC ) = 
	  PADS_BANK0_GPIO0_OD(1) 
	| PADS_BANK0_GPIO0_IE(1) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(1) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( BTN_LOC ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);
}

_Bool button_is_down(){
    return !button_is_up();
}

_Bool button_is_up(){
    return SIO_GPIO_IN & (1<<7);
}

button_press_t get_button_press()
{
    static uint16_t cntr;
    static uint8_t debounce_cntr;
    static button_press_t state;
    button_press_t retval=NO_PRESS;
    if( debounce_cntr == 5 ){
        debounce_cntr = 0;
    }
    else{
        debounce_cntr++;
        return retval;
    }

    switch(state) {
		case NO_PRESS:
			if( button_is_down() ){
                cntr = 0;
                state = SHORT_PRESS;
            }
			break;
		case SHORT_PRESS:
			if( button_is_down() ){
                if( cntr >= HALF_SECOND ){
                    state = PRESS;
                }
                else{ state = SHORT_PRESS; }
                cntr++;
            }
            if( button_is_up() ){
                retval = SHORT_PRESS;
                state = NO_PRESS;
            }
			break;
		case PRESS:
            if( button_is_down() ){
                if( cntr >= FULL_SECOND ){
                    state = LONG_PRESS;
                }
                else{ state = PRESS; }
                cntr++;
            }
            if( button_is_up() ){
                retval = PRESS;
                state = NO_PRESS;
            }
			break;
		case LONG_PRESS:
			if( button_is_up() ){
                retval = LONG_PRESS;
                state = NO_PRESS;
            }
            else{ state = LONG_PRESS; }
			break;
	}
    return retval;
}
