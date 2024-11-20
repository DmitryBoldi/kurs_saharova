

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>


void uart_setup(void) {

  //  rcc_priph_clock_enable(RCC_USART1);

    // Настраиваем пины PA2 (TX) и PA3 (RX) для USART2
        // Включаем тактирование для GPIOA и USART2
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2); // TX
    gpio_set_af(GPIOA, GPIO_AF7, GPIO3); // RX

    // Настраиваем USART2
    usart_set_baudrate(USART2, 19200); // Устанавливаем скорость бит/с 
    usart_set_databits(USART2, 8); // 8 бит данных
    usart_set_stopbits(USART2, USART_STOPBITS_1); // 1 стоп-бит
    usart_set_mode(USART2, USART_MODE_TX_RX); // Режим TX и RX
    usart_set_parity(USART2, USART_PARITY_NONE); // Без четности
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE); // Без управления потоком

    usart_enable_rx_interrupt(USART2);
    nvic_enable_irq(NVIC_USART2_IRQ);
    // Включаем USART2
    usart_enable(USART2);
}


void usart_transmit(void) {
    // Ожидаем, пока данные не будут доступны

usart_send_blocking(USART2, 0x55);
usart_send_blocking(USART2, 'H');
usart_send_blocking(USART2, 'e');
usart_send_blocking(USART2, 'l');
usart_send_blocking(USART2, 'l');
usart_send_blocking(USART2, 'o');
usart_send_blocking(USART2, '\n');
usart_send_blocking(USART2, '\r');

}






int main(void)  {

    uart_setup();
    usart_transmit();
    rcc_periph_clock_enable(RCC_GPIOE);
    gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
    
    

    while (1) {

         gpio_toggle(GPIOE, GPIO15);
        // Отправка полученных данных обратно
        for (volatile int i = 0; i < 1000000; i++);
    }
}



