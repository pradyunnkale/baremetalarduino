#include <stdint.h>
#include <stddef.h>

void my_delay(uint32_t count);

int main() {
    //Set PIN 13 to output
    *(volatile uint8_t*)(0x24) |= (1 << 5);
    //Set PIN 2 to input
    *(volatile uint8_t*)(0x2A) &= ~(1 << 2);
    //Set pullup resistor
    *(volatile uint8_t*)(0x2B) |= (1 << 2);
    uint8_t curr;
    uint8_t prev = 1;
    while (1) {
        curr = (*(volatile uint8_t*)(0x29) & (1 << 2)) ? 1 : 0;
        if (prev == 1 && curr == 0) {
            *(volatile uint8_t*)(0x25) ^= (1 << 5);
            my_delay(10);
        }
        prev = curr;
    }

    return 0;
}

void my_delay(uint32_t count) {
    while (count) {
        asm("nop");
        count--;
    }
}
