#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include <stdbool.h>
#include <stdint.h>
#include "keypad.h"

#define CONCAT2(X,Y) X ## Y
#define CONCAT3(X,Y,Z) X ## Y ## Z
#define IO_BANK0_GPIO_CTRL(X) CONCAT3(IO_BANK0_GPIO,X,_CTRL)
#define PADS_BANK0_GPIO(X) CONCAT2(PADS_BANK0_GPIO,X)
#define GPIO_RESETS (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK)
#define R0 16
#define R1 17
#define R2 18
#define R3 19
#define C0 15
#define C1 14
#define C2 13
#define NO_KEY 15
#define ALL 15

static char keypad_char;
typedef enum {WAIT_PRESS, SCAN0, SCAN1, SCAN2, SCAN3, WAIT_RELEASE} keypad_t;
const char keypad_lut[] = {'1','2','3','4','5','6','7','8','9','*','0','#'};

void configure_keypad( void )
{
    keypad_char = 0;

	resets.reset_clr = GPIO_RESETS;
    while(! (resets.reset_done & GPIO_RESETS) )
	    continue;

	PADS_BANK0_GPIO( R0 ) = 
	  PADS_BANK0_GPIO0_OD(0) 
	| PADS_BANK0_GPIO0_IE(0) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(0) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( R0 ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

	PADS_BANK0_GPIO( R1 ) = 
	  PADS_BANK0_GPIO0_OD(0) 
	| PADS_BANK0_GPIO0_IE(0) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(0) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( R1 ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

	PADS_BANK0_GPIO( R2 ) = 
	  PADS_BANK0_GPIO0_OD(0) 
	| PADS_BANK0_GPIO0_IE(0) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(0) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( R2 ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

	PADS_BANK0_GPIO( R3 ) = 
	  PADS_BANK0_GPIO0_OD(0) 
	| PADS_BANK0_GPIO0_IE(0) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(0) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( R3 ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

	PADS_BANK0_GPIO( C0 ) = 
	  PADS_BANK0_GPIO0_OD(1) 
	| PADS_BANK0_GPIO0_IE(1) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(1) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( C0 ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

	PADS_BANK0_GPIO( C1 ) = 
	  PADS_BANK0_GPIO0_OD(1) 
	| PADS_BANK0_GPIO0_IE(1) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(1) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( C1 ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

	PADS_BANK0_GPIO( C2 ) = 
	  PADS_BANK0_GPIO0_OD(1) 
	| PADS_BANK0_GPIO0_IE(1) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(1) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( C2 ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

	keypad_scan_row(ALL);
}

void task_keypad_scan() // FSM
{
	static keypad_t state = WAIT_PRESS;
	uint8_t col = read_col();
	switch(state) {
		case WAIT_PRESS:
			if( col != NO_KEY ){
                keypad_scan_row(0);
                state = SCAN0;
            }
			break;
		case SCAN0:
			if( col == NO_KEY ){
                keypad_scan_row(1);
                state = SCAN1;
            }
            else{
                keypad_scan_row(ALL);
				keypad_char = keypad_lut[0+col];
				state = WAIT_RELEASE;
            }
			break;
		case SCAN1:
			if( col == NO_KEY ){
                keypad_scan_row(2);
                state = SCAN2;
            }
            else{
                keypad_scan_row(ALL);
				keypad_char = keypad_lut[3+col];
				state = WAIT_RELEASE;
            }
			break;
		case SCAN2:
			if( col == NO_KEY ){
                keypad_scan_row(3);
                state = SCAN3;
            }
            else{
                keypad_scan_row(ALL);
				keypad_char = keypad_lut[6+col];
				state = WAIT_RELEASE;
            }
			break;
		case SCAN3:
			if( col == NO_KEY ){
                keypad_scan_row(ALL);
                state = WAIT_RELEASE;
            }
            else{
                keypad_scan_row(ALL);
				keypad_char = keypad_lut[9+col];
				state = WAIT_RELEASE;
            }
			break;
		case WAIT_RELEASE:
			if( col == NO_KEY ){
				state = WAIT_PRESS;
				keypad_scan_row(ALL);
			}
	}
}

int keypad_getchar( char *cp )
{
	*cp = keypad_char;
	if( *cp == 0 ){
		return 0;
	}
	else{
		keypad_char = 0;
		return 1;
	}
}

void keypad_scan_row(int8_t row)
{
	SIO_GPIO_OUT_CLR = (1<<R0)|(1<<R1)|(1<<R2)|(1<<R3);
	SIO_GPIO_OE_CLR = (1<<R0)|(1<<R1)|(1<<R2)|(1<<R3); // ZZZZ
	switch( row )
	{
		case -1: break;
		case 0: SIO_GPIO_OE_SET = (1<<R0); // ZZZ0
				break;
		case 1: SIO_GPIO_OE_SET = (1<<R1); // ZZ0Z
				break;
		case 2: SIO_GPIO_OE_SET = (1<<R2); // Z0ZZ
				break;
		case 3: SIO_GPIO_OE_SET = (1<<R3); // 0ZZZ
				break;
		default: SIO_GPIO_OE_SET = (1<<R0)|(1<<R1)|(1<<R2)|(1<<R3); // 0000
				break;
	}
}

uint8_t read_col()
{
	uint8_t retval = NO_KEY;
	if( 0 == (SIO_GPIO_IN & (1<<C0)) )
		retval=0;
	else if( 0 == (SIO_GPIO_IN & (1<<C1)) )
		retval=1;
	else if( 0 == (SIO_GPIO_IN & (1<<C2)) )
		retval=2;
	return retval;
}

