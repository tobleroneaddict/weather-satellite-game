#ifndef __SDCC
#define __interrupt(x)
#endif

#include <stdint.h>

//Z80
//Address space will be:


float *flag;


void isr_nmi(void) __interrupt(1)
{
    *(uint8_t*)0x3010 = 'N';
}

void main(void) {
    flag = (float*)0x3000;
    float meow = 30.5;



    while (1) {
        *flag = (int)meow;
    }
}