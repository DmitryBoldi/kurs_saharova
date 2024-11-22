#include "ring_buffer/ring_buf.hpp"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>


uint8_t c{'a'};

ring_buf buf; 



void usart2_exti26_isr(void){

USART_RQR(USART2) &= ~(USART_RQR_RXFRQ);

//static uint8_t c = static_cast<uint8_t>(usart_recv(USART2));

if (buf.not_full()){
        buf.put(static_cast<uint8_t>(usart_recv(USART2)));
}

gpio_toggle (GPIOE, GPIO14);
    //Очистить флаг запроса прерывания
    //Сохранить принятый символ в переменную
    //Переключить светодиод 
 }


void bufer(void){

}


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


void setup_LED(void){
    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15| GPIO14|  GPIO13| GPIO12);
    gpio_set(GPIOD, GPIO15|GPIO12);
}



void led_blink(uint32_t tome) {
      gpio_clear(GPIOD, GPIO15); 
        for (volatile uint32_t i = 0; i < tome*1000; i++); 
        gpio_set(GPIOD, GPIO15);
        for (volatile uint32_t i = 0; i < tome*1000; i++);     // Задержка
}


void loop(){
if (!buf.empty()){c = buf.get();}
usart_send_blocking (USART2 , c);
for (volatile uint32_t i=0;i<20000;i++);
gpio_toggle(GPIOE, GPIO9);

}



int main(void)  {

    uart_setup();
    setup_LED();
    usart_transmit();
    


  


    while (1) {
       loop();
      led_blink(400);
         
    }
}



