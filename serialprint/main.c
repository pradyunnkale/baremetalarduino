#include <stdint.h>
#include <stddef.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD) - 1) 

void uart_init(void);
void uart_transmit(uint8_t data);
void uart_print(char* str);
void my_delay(uint32_t count);

int main() {   
    uart_init(); 
    
    while (1) {
        uart_print("Hello World!\r\n");
        my_delay(100000);
    }
    return 0;
}

void uart_init(void) {
    //Set baud rate
    *(volatile uint8_t*)(0xC5) = (UBRR_VALUE >> 8); //Sets UART high
    *(volatile uint8_t*)(0xC4) = UBRR_VALUE; //Sets UART low

    //Enable transmission
    *(volatile uint8_t*)(0xC1) = (1 << 3);
    
    //Set frame
    *(volatile uint8_t*)(0xC2) = (1 << 2) | (1 << 1);
}

void uart_transmit(uint8_t data) {
    while(!((*(volatile uint8_t*)(0xC0)) & (1 << 5)));

    *(volatile uint8_t*)(0xC6) = data;
}

void uart_print(char* str) {
    while (*str != '\0') {
        uart_transmit(*str);
        str++;
    }
}

void my_delay(uint32_t count) {
    while (count) {
        asm("nop");
        count--;
    }
}
