#ifndef __keypad_i2c_
#define __keypad_i2c_

#include "transfer_handler.h"

void keypad_begin(void);
void get_keyval(uint8_t* keymap, int16_t* encpos);

#endif

