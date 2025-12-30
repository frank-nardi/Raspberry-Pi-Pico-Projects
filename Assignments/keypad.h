#ifndef KEYPAD_H
#define KEYPAD_H

void configure_keypad( void );
void task_keypad_scan();
int keypad_getchar( char *cp );
void keypad_scan_row(int8_t row);
uint8_t read_col();

#endif
