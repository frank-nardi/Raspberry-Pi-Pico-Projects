#include "systick.h"
#include "led.h"
#include <stdint.h>
#include "button.h"

void main( void )
{
	configure_led();
	configure_systick();
    configure_button();
    button_press_t status;
    button_press_t last_status = NO_PRESS;

    _Bool pulse_led4();

	while(1){
		if( !system_tick() )
			continue;

        status = get_button_press();

        if( status == SHORT_PRESS ){
            last_status = SHORT_PRESS;
            turn_off_led();
        }
        if( status == PRESS ){
            last_status = PRESS;
            turn_on_led();
        }
        if( status == LONG_PRESS ){
            last_status = LONG_PRESS;
        }
        if( last_status == LONG_PRESS ){
            if( pulse_led4() == 0 ){
                last_status = NO_PRESS;
            }
        }
	}
}

_Bool pulse_led4()
{
    static uint8_t cntr;
	static uint8_t blinks;
	static enum {GO, STOP} state=GO;

	cntr++;

	switch(state) {
		case GO:
			if( blinks == 4 ){
				blinks = 0;
				state = STOP;
			}
			if( cntr == 35 )
			{ turn_on_led(); }
			if( cntr == 100 ){
				turn_off_led();
				cntr = 0;
				blinks++;
			}
			return 1;
			break;
		case STOP:
			state = GO;
			return 0;
			break;
	}
	return 0;
}
