#include "encoder.h"
#include <rp2350/resets.h>
#include <rp2350/sio.h>
#include <rp2350/io_bank0.h>
#include <rp2350/pads_bank0.h>
#include "gpio_irq.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define A_LOC 14
#define B_LOC 15
#define BTN_LOC 16
#define ENC_A (get_pin14())
#define ENC_B (get_pin15())
#define CONCAT2(X,Y) X ## Y
#define CONCAT3(X,Y,Z) X ## Y ## Z
#define IO_BANK0_GPIO_CTRL(X) CONCAT3(IO_BANK0_GPIO,X,_CTRL)
#define PADS_BANK0_GPIO(X) CONCAT2(PADS_BANK0_GPIO,X)
#define GPIO_RESETS (RESETS_RESET_IO_BANK0_MASK | RESETS_RESET_PADS_BANK0_MASK)

static uint8_t new_pos;
static uint8_t last_pos;
static uint8_t pos;

static uint8_t num_turns;

static void callback_pin_14();
static void callback_pin_15();
static _Bool pin14_state;
static _Bool pin15_state;

void configure_encoder( void )
{
    resets.reset_clr = GPIO_RESETS;
    while(! (resets.reset_done & GPIO_RESETS) )
	    continue;

    PADS_BANK0_GPIO( A_LOC ) = 
	  PADS_BANK0_GPIO0_OD(1) 
	| PADS_BANK0_GPIO0_IE(1) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(1) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( A_LOC ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

    PADS_BANK0_GPIO( B_LOC ) = 
	  PADS_BANK0_GPIO0_OD(1) 
	| PADS_BANK0_GPIO0_IE(1) 
	| PADS_BANK0_GPIO0_DRIVE(0) 
	| PADS_BANK0_GPIO0_PUE(1) 
	| PADS_BANK0_GPIO0_PDE(0) 
	| PADS_BANK0_GPIO0_SCHMITT(0) 
	| PADS_BANK0_GPIO0_SLEWFAST(0);
    IO_BANK0_GPIO_CTRL( B_LOC ) = 
	  IO_BANK0_GPIO0_CTRL_IRQOVER(0) 
    | IO_BANK0_GPIO0_CTRL_INOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OEOVER(0)  
    | IO_BANK0_GPIO0_CTRL_OUTOVER(0) 
    | IO_BANK0_GPIO0_CTRL_FUNCSEL(5);

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

    register_gpio_irq_callback(callback_pin_14);
    register_gpio_irq_callback(callback_pin_15);

	io_bank0.proc0_inte1 |= ( IO_BANK0_PROC0_INTE1_GPIO14_EDGE_HIGH_MASK
		             |IO_BANK0_PROC0_INTE1_GPIO14_EDGE_LOW_MASK
                     |IO_BANK0_PROC0_INTE1_GPIO15_EDGE_HIGH_MASK
                     |IO_BANK0_PROC0_INTE1_GPIO15_EDGE_LOW_MASK); 
}

_Bool get_pin14(){
	return pin14_state;
}
_Bool get_pin15(){
	return pin15_state;
}
_Bool get_pin16(){
    return SIO_GPIO_IN & (1<<BTN_LOC);
}

static void callback_pin_14( ){
	uint8_t events = get_irq_status_for_pin(14);
	if( !events )
		return;
	if( events & IRQ_EDGE_HIGH )
		pin14_state=true;
	if( events & IRQ_EDGE_LOW )
		pin14_state=false;
}

static void callback_pin_15( ){
    events = get_irq_status_for_pin(15);
	if( !events )
		return;
	if( events & IRQ_EDGE_HIGH )
		pin15_state=true;
	if( events & IRQ_EDGE_LOW )
		pin15_state=false;
}


void determine_position(){
    new_pos = gray_to_binary();

    int diff = last_pos - new_pos;

    if( diff == -1 || diff == 3 ){
        last_pos = new_pos;
        if( pos < 100 ) { pos++; }
    }
    else if( diff == 1 || diff == -3 ){
        last_pos = new_pos;
        if( pos > 0)  { pos--; }
    }
    else{
    }
}

int gray_to_binary(){
    int ret;
    num_turns++;
    if( ENC_A == 0 && ENC_B == 0 ){ ret=0; }
    else if ( ENC_A == 0 && ENC_B == 1 ){ ret=1; }
    else if ( ENC_A == 1 && ENC_B == 1 ){ ret=2; }
    else if ( ENC_A == 1 && ENC_B == 0 ){ ret=3; }
    return ret;
}

uint8_t get_pos(){
    return pos;
}

uint8_t get_num_turns(){
    return num_turns;
}

void reset_num_turns(){
    num_turns = 0;
}
