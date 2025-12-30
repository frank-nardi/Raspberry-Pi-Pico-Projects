#ifndef ENCODER_H
#define ENCODER_H

#include <stdbool.h>
#include <stdint.h>

void configure_encoder( void );
_Bool get_pin14();
_Bool get_pin15();
_Bool get_pin16();
void determine_position();
int gray_to_binary();
uint8_t get_pos();
uint8_t get_num_turns();
void reset_num_turns();

#endif
