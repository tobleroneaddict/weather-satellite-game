#ifndef __SDCC
#define __interrupt(x)
#endif

#include <stdint.h>

//Z80


float *flag;


void isr_nmi(void) __interrupt(1)
{
    *(uint8_t*)0x8010 = 'N';
}

void main(void) {
    flag = (float*)0x8000;
    float meow = 30.5;



    while (1) {
        *flag = (int)meow;
    }
}