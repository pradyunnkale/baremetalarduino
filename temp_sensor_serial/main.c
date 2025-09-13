#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD) - 1) 

void uart_init(void);
void uart_transmit(uint8_t data);
void uart_print(char* str);
void my_delay(uint32_t count);

void adc_init(void);
uint16_t adc_read(void);
void uart_init(void);

int main() {

    adc_init();
    uart_init();
    // Set A0 input
    *(uint8_t volatile*)(0x27) &= ~(1 << 0);

    uint16_t data;
    float voltage;
    float temp_c;
    char buf[11];
    uint16_t temp_int;
    uint16_t temp_frac;
    while (1) {
        data = adc_read();
        voltage = data * 5.0f / 1023.0f;
        temp_c = voltage * 100.0f;
        temp_int = (uint16_t)(temp_c);
        temp_frac = (uint16_t)((temp_c - temp_int)*100);
        sprintf(buf, "%d.%02d", temp_int, temp_frac);
        uart_print(buf);
        uart_print("\r\n");
        my_delay(100000);
    }

    return 0;
}


void adc_init(void) {
    //Set the MUX register
    *(uint8_t volatile*)(0x7C) = 0x40;
    //Sets the status of the adc
    *(uint8_t volatile*)(0x7A) = 0x87;
}

uint16_t adc_read(void) {
    *(uint8_t volatile*)(0x7A) |= (1 << 6);
    
    while (*(uint8_t volatile*)(0x7A) & (1 << 6));

    uint16_t data = *(volatile uint8_t*)(0x78); 
    data |= (*(volatile uint8_t*)(0x79) << 8);
    return data;
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
