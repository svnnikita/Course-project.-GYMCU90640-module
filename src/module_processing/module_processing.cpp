#include <libopencm3/stm32/rcc.h> 
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "module_processing/module_processing.hpp"

#define BAUD_SPEED 115200
#define WORD_SIZE 8

// Тактирование
void Clock_Setup(void) {
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_64MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);
    rcc_periph_clock_enable(RCC_USART2);
}

// Настройка UART2 для приема данных с датчика и передачи их в терминал
void UART2_GYMCU90640_Setup(void) {
    // Настраиваем выводы
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2|GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2|GPIO3);

    // Настраиваем UART
    usart_set_baudrate(USART2, BAUD_SPEED);
    usart_set_databits(USART2, WORD_SIZE);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    // Включаем прерывания для USART2 
    usart_enable_rx_interrupt(USART2);
    nvic_enable_irq(NVIC_USART2_EXTI26_IRQ);

    // Включаем UART
    usart_enable(USART2);
}

// Настраиваем UART1, по которому будем отправлять команды датчику
void UART1_Data_Setup(void) {
    // Настраиваем выводы
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9);

    // Настраиваем UART
    usart_set_baudrate(USART1, BAUD_SPEED);
    usart_set_databits(USART1, WORD_SIZE);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    // Включаем UART1
    usart_enable(USART1);
}

// Отправляем команды датчику
void UART_Send_Command(uint8_t *command) {
    for (int i = 0; i < 4; i++) {
        usart_send_blocking(USART1, command[i]);
    }
}

// Обрабатываем данные с датчика
static uint16_t index1 = 0; 
void UART2_Processing(sensor_data *sensor) {
    
    if (usart_get_flag(USART2, USART_FLAG_RXNE)) {
        sensor->frame_buffer[index1] = usart_recv(USART2);
        index1++;
        
        if (index1 >= DATA_SIZE) {
            
            for (uint16_t i = 0; i < DATA_SIZE; i++) {
                // Ждем, пока буфер передачи не освободится
                while (!usart_get_flag(USART2, USART_FLAG_TXE)) {
                }
                usart_send(USART2, sensor->frame_buffer[i]);
            }
            sensor->frame_ready = true;
            // Сбрасываем индекс для следующего фрейма
            index1 = 0;
        }
    }
}