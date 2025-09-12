#include <stdint.h>
#include <stddef.h>

void my_delay(uint32_t count);

int main(void) {
    
    *(uint8_t volatile*)(0x24) |= (1 << 5);
    

    while (1) {
        *(uint8_t volatile*)(0x25) |= (1 << 5);
        my_delay(1000000);
        *(uint8_t volatile*)(0x25) &= ~(1 << 5);
        my_delay(1000000);
    }

    return 0;
}

void my_delay(uint32_t count) {
    while (count--) {
        __asm__ __volatile__ ("nop");
    }
}
