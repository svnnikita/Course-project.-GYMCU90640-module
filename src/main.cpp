#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "module_processing/module_processing.hpp"

sensor_data sensor = {};

void usart2_exti26_isr(void) {
    UART2_Processing(&sensor);
}

int main(void) {
    Clock_Setup();
    UART1_Data_Setup();
    UART2_GYMCU90640_Setup();
    
    // Настраиваем частоту отправки кадров 2 Гц
    uint8_t cmd2[] = {0xA5, 0x25, 0x02, 0xCC};
    UART_Send_Command(cmd2);

    for (volatile uint32_t i = 0; i < 100000; i++) {
        __asm__("nop");
    }

    while (1) {
    }

    return 0;
}