#include "usbcdc.h"
#include "encoder.h"
#include "systick.h"
#include "spi0.h"
#include "timer0.h"
#include "watchdog.h"
#include "interrupt.h" // gpio_irq.h
#include <stdio.h>
#include <stdint.h>
#include <rp2350/spi.h>
#include <rp2350/rosc.h>
#include <rp2350/watchdog.h>
#include <rp2350/psm.h>

#define X_VAL_REG 0x28
#define READ_CMD (1<<7)
#define WAIT 75
#define WATCHDOG_TIME_SEL 10000000

void get_accel_val();
void game_control();
uint8_t gen_random_bits();
void out_of_time();
void new_rand_state();

int (* _stdin_getchar)( char *c) = usbcdc_getchar;
int (* _stdout_putchar)( char c) = usbcdc_putchar;

typedef enum {CONFIG, WAIT_CONFIG, WRITE, WAIT_TX_DONE, READ, WAIT_RX_DONE} command_t;
typedef enum {START, TWIST, TAP, SHAKE} game_t;
static game_t game_state;
static _Bool print_text;

static uint8_t reg_val;
static char c;
static _Bool button_pressed;
static int score=-1;
static uint8_t button_debouncer;
static uint8_t num_turns;


int main()
{
	__disable_irq();

	configure_systick();
	configure_encoder();
	configure_usbcdc();
	configure_spi0();
	configure_timer0();
	configure_watchdog(WATCHDOG_TIME_SEL);
	systick_register_callback(/*SOMETHING*/);

    __enable_irq();

	while(1){
        __asm__("WFI");
        if(!system_tick())
            continue; 
		game_control();
		get_accel_val();
		if( button_debouncer ==  5 ){
			button_pressed = !get_pin16();
			num_turns = get_num_turns();
			button_debouncer = 0;
		}
		button_debouncer++;
	}
}

void get_accel_val(){
	static command_t state;
	static int wait;
	switch(state){
	case CONFIG:
		spi0_write(0 | (1<<6) | 0x1e);
		spi0_write(0x90);
		spi0_write(0x00);
		spi0_write(0x57);
		spi0_write(0x00);
		spi0_write(0x00);
		spi0_write(0x00);
		state = WAIT_CONFIG;
		break;
	case WAIT_CONFIG:
		if(wait == 5000){
			wait = 0;
			state = READ;
		}
		else{ wait++; }
		break;
	case WRITE:
		spi0_write(READ_CMD | 0x29);
		spi0_write(0x00);
		state = WAIT_TX_DONE;
		break;
	case WAIT_TX_DONE:
		if(wait == WAIT){
			wait = 0;
			state = READ;
		}
		else{ wait++; }
		break;
	case READ:
		if( spi0_read(&reg_val) ){
			spi0_read(&reg_val);
			if( reg_val == 0xFF ){
				reg_val = 0x00;
			}
		}
		else{state = WAIT_RX_DONE;}
		break;
	case WAIT_RX_DONE:
		if(wait == WAIT){
			wait = 0;
			state = WRITE;
		}
		else{ wait++; }
		break;
	}
}

void game_control() {
	feed_the_watchdog();
	switch(game_state) {
		case START:
		if( print_text ){
				printf("START!\n\r");
				print_text = 0;
			}	
		if( usbcdc_getchar(&c) && (c == ' ') ){
				c = '.';
				score = 0;
				new_rand_state();
			}
			break;
		case TWIST:
			if( print_text ){
				printf("Twist!\n\r");
				print_text = 0;
				reset_num_turns();
				set_alarm(0, 3000000-(score*10000), out_of_time, 5<<4);
			}
			if( num_turns >= 50 ){ 
				reset_num_turns();
				set_alarm(0, 250000, new_rand_state, 4<<4);
			}
			break;
		case TAP:
			if( print_text ){
				printf("Tap!\n\r");
				print_text = 0;
				set_alarm(1, 3000000-(score*10000), out_of_time, 5<<4);
			}
			if( button_pressed ){
				set_alarm(1, 250000, new_rand_state, 4<<4);
			}
			break;
		case SHAKE:
			if( print_text ){
				printf("Shake!\n\r");
				print_text = 0;
				set_alarm(2, 3000000-(score*10000), out_of_time, 5<<4);
			}
			if( reg_val >= 0xF0 ){
				set_alarm(2, 250000, new_rand_state, 4<<4);
			}
			break;
	}
}

uint8_t gen_random_bits() {
	uint8_t int1;
	uint8_t int2;
	int1 = ROSC_RANDOMBIT & (1<<0);
	for(int i=0;i<1000;i++){
		if( i == 999 )
		{ int2 = ROSC_RANDOMBIT & (1<<0); }
	}
	if( int1 == 0 ){
		if( int2 == 0 ){
			return 0;
		}
		else{ return 1; }
	}
	else{
		if( int2 == 0 ){
			return 2;
		}
	}
	return (2 - (ROSC_RANDOMBIT & (1<<0)));
}

void out_of_time()
{
	game_state = START;
	printf("|  You Lose  |\n\rScore: %d\n\r", score);
}

void new_rand_state()
{
	score++;
	print_text = 1;
	uint8_t rand = gen_random_bits();
	switch(rand){
		case 0:
			game_state = TWIST;
			break;
		case 1:
			game_state = TAP;
			break;
		case 2:
			game_state = SHAKE;
			break;
		case 3:
			game_state = START;
			break;
	}
	print_text = 1;
}
