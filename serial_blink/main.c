#include <stdint.h>
#include <stddef.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD) - 1) 


void uart_init(void);
void uart_transmit(uint8_t data);
uint8_t uart_receive(void);
void uart_print(char* str);
void my_delay(uint32_t count);

int main() {

    uart_init();
    //Sets pin 13 as output
    *(uint8_t volatile*)(0x24) |= (1 << 5);

    uint8_t cmd;

    while (1) { 
        cmd = uart_receive();
        uart_print("Command received: ");
        uart_transmit(cmd);
        uart_print("\r\n");

        if (cmd == 'l') {
            // Toggles led
            *(uint8_t volatile*)(0x25) ^= (1 << 5);
            if (*(uint8_t volatile*)(0x25) & (1 << 5)) {
                uart_print("LED ON\r\n");
            } else {
                uart_print("LED OFF\r\n");
            }
        } else {
            uart_print("INVALID COMMAND\r\n");
        }
    }
    return 0;
}


void uart_init(void) {
    //Set baud rate
    *(volatile uint8_t*)(0xC5) = (UBRR_VALUE >> 8); //Sets UART high
    *(volatile uint8_t*)(0xC4) = UBRR_VALUE; //Sets UART low

    //Enable transmission
    *(volatile uint8_t*)(0xC1) = (1 << 3) | (1 << 4);
    
    //Set frame
    *(volatile uint8_t*)(0xC2) = (1 << 2) | (1 << 1);
}

void uart_transmit(uint8_t data) {
    while(!((*(volatile uint8_t*)(0xC0)) & (1 << 5)));
    // Sets data byte
    *(volatile uint8_t*)(0xC6) = data;
}

uint8_t uart_receive(void) {
    while(!(*(volatile uint8_t*)(0xC0) & (1 << 7)));
    // Reads data byte
    uint8_t data = *(volatile uint8_t*)(0xC6); 
    return data;
}

void uart_print(char* str) {
    // Lets console print strings
    while (*str != '\0') {
        uart_transmit(*str);
        str++;
    }
}

void my_delay(uint32_t count) {
    // Delays by cpu cycle
    while (count) {
        asm("nop");
        count--;
    }
}
