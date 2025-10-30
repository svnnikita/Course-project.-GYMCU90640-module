#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "module_processing/module_processing.hpp"

// СХЕМА ПОДКЛЮЧЕНИЯ:
// CONNECTION DIAGRAM:
// USART2_TX (PA2) <--> RXD USB2TTL
// USART2_RX (PA3) <--> TX MODULE
// USART1_TX (PA9) <--> RX MODULE
// ОБЪЕДИНИТЬ ВСЕ ЗЕМЛИ
// UNITE ALL LANDS

sensor_data sensor = {0};

void usart2_exti26_isr(void) {
    UART2_Processing(&sensor);
}

int main(void) {
    Clock_Setup();
    UART1_Data_Setup();
    UART2_GYMCU90640_Setup();   
    
    for (volatile uint32_t i = 0; i < 100000; i++) {
        __asm__("nop");
    }
    
    while (1) {
    }

    return 0;
}