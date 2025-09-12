#include <stdint.h>
#include <stddef.h>

void my_delay(uint32_t count); //Function prototype

int main(void) {
    
    *(uint8_t volatile*)(0x24) |= (1 << 5); //Sets pin 13 to output 
    

    while (1) {
        *(uint8_t volatile*)(0x25) |= (1 << 5); //Turns on LED
        my_delay(1000000); //Delay is 3-4 million cpu cycles
        *(uint8_t volatile*)(0x25) &= ~(1 << 5); //Turns off LED
        my_delay(1000000);
    }

    return 0;
}

void my_delay(uint32_t count) { //Makeshift delay function
    while (count--) {
        __asm__ __volatile__ ("nop");
    }
}
