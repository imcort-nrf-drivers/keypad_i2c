#include "keypad_i2c.h"

#define PCF8575_ADDR 0x20

uint16_t _PORT;
int16_t enc_position = 0;

uint8_t last_s;

uint16_t keypad_read(void)
{
    
    uint8_t readbuf[2];
    iic_read(PCF8575_ADDR, readbuf, 2);
    
    _PORT = readbuf[0] | (readbuf[1] << 8);
    
    uint8_t s = last_s & 0x0c; //0b1100
    s |= ((readbuf[0] & 0x20) >> 4) | ((readbuf[0] & 0x04) >> 2); //SWA | SWB
    
    switch (s) {
        case 0: case 5: case 10: case 15:
            break;
        case 2: case 4: case 11: case 13:
            enc_position++;
        break;
        case 1: case 7: case 8: case 14:
            enc_position--;
        break;
        case 3: case 12:
            enc_position += 2;
        break;
        default:
            enc_position -= 2;
        break;
    }
    
    last_s = s << 2;
    
    Debug("Keypad %x %x %d %d", readbuf[0], readbuf[1], enc_position / 4);
    
    return _PORT;

}
 
void keypad_begin(void)
{

    iic_init();
    pinMode(KEYPAD_INT, INPUT);
    
    uint8_t sendbuf[2] = {0xff, 0xff};
    iic_send(PCF8575_ADDR, sendbuf, 2, false);
    attachInterrupt(KEYPAD_INT, keypad_read, FALLING);

}

void get_keyval(uint8_t* keymap, int16_t* encpos)
{
    
    *keymap = 0;
    *keymap |= (_PORT & 0x0010) >> 4; //KEY0
    *keymap |= (_PORT & 0x0002) >> 0; //KEY1
    *keymap |= (_PORT & 0x0040) >> 4; //KEY2
    *keymap |= (_PORT & 0x0001) << 3; //KEY3
    *keymap |= (_PORT & 0x0008) << 1; //KEY4
    *keymap |= (_PORT & 0x8000) >> 10; //KEY5
    *keymap |= (_PORT & 0x0080) >> 1; //KEY6
    
    *keymap = ~(*keymap);
    
    *encpos = enc_position;

}
