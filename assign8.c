#include "usbcdc.h"
#include "spi0.h"
#include <stdio.h>
#include <stdint.h>
#include <rp2350/spi.h>

#define READ_CMD (1<<7)
#define WRITE_CMD 0
#define RW_MULTIPLE (1<<6)
#define DUMMY 0

int (* _stdin_getchar)( char *c) = usbcdc_getchar;
int (* _stdout_putchar)( char c) = usbcdc_putchar;

typedef enum {WAIT, READ, WRITE, CLEAR} command_t;

int hex_to_int(char c);

int main()
{
	configure_spi0();
	configure_usbcdc();

	while(1)
	{
		char c;
		if( usbcdc_getchar(&c) && c != '\n' )
		{
			static uint8_t reg;
			static uint8_t reg_val;
			static uint8_t address_digits_count;
			static command_t state;
			switch(state) {
			case READ:
				if( address_digits_count == 2 ){
					if( spi0_read(&reg_val) ){
						spi0_read(&reg_val);
					}
				}
				if( address_digits_count == 2 && c == '\r' ){
					printf("The value of register 0x%02X is 0x%02X\n\r", reg, reg_val);
					state = WAIT;
				}
				if( address_digits_count == 1 ){
					usbcdc_putchar(c);
					reg += hex_to_int(c);
					spi0_write(READ_CMD | reg);
					spi0_write(DUMMY);
					address_digits_count++;
				}
				if( address_digits_count == 0 ){
					reg += (16 * hex_to_int(c));
					usbcdc_putchar(c);
					address_digits_count++;
				}
				break;
			case WRITE:
				if( address_digits_count == 4 && c == '\r' ){
					printf("The value written to register 0x%02X is 0x%02X\n\r", reg, reg_val);
					state = WAIT;
				}
				if( address_digits_count == 3 ){
					usbcdc_putchar(c);
					printf("\n\r%d\n\r", hex_to_int(c));
					reg_val += hex_to_int(c);
					spi0_write(WRITE_CMD | reg);
					spi0_write(reg_val);
					address_digits_count++;
				}
				if( address_digits_count == 2 ){
					printf("\n\r%d\n\r", hex_to_int(c));
					reg_val += (16 * hex_to_int(c));
					usbcdc_putchar(c);
					address_digits_count++;
				}
				if( address_digits_count == 1 ){
					printf("\n\r%d\n\r", hex_to_int(c));
					reg += hex_to_int(c);
					usbcdc_putchar(c);
					address_digits_count++;
				}
				if( address_digits_count == 0 ){
					printf("\n\r%d\n\r", hex_to_int(c));
					reg += (16 * hex_to_int(c));
					usbcdc_putchar(c);
					address_digits_count++;
				}
				break;
			case WAIT:
				if( c == 'R' || c == 'r' ){
					reg = 0;
					reg_val = 0;
					address_digits_count = 0;
					usbcdc_putchar(c);
					state = READ;
				}
				if( c == 'W' || c == 'w' ){
					reg = 0;
					reg_val = 0;
					address_digits_count = 0;
					usbcdc_putchar(c);
					state = WRITE;
				}
				if( c == 'C' || c == 'c' ){
					reg = 0;
					reg_val = 0;
					address_digits_count = 0;
					usbcdc_putchar(c);
					state = CLEAR;
				}
				break;
			case CLEAR:
				while(spi0_read(&reg_val))
				{}
				state = WAIT;
				break;
			}	
		}
	}
	return 0;
}

int hex_to_int(char c)
{
	if( c >= 48 && c <= 57 ){
		return c - 48;
	}
	else{
		switch(c) {
			case 'A':
			case 'a':
				return 10;
				break;
			case 'B':
			case 'b':
				return 11;
				break;
			case 'C':
			case 'c':
				return 12;
				break;
			case 'D':
			case 'd':
				return 13;
				break;
			case 'E':
			case 'e':
				return 14;
				break;
			case 'F':
			case 'f':
				return 15;
				break;
		}
	}
	return 0;
}
