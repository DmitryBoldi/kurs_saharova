
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>


#include "/home/dmitry/Documents/kurs_saharova1/ring_buffer/Ring_buf.hpp"

void read_data_UART(void);


uint8_t GPS_info ;

Ring_buffer buf; 



// Функция usart2_exti26_isr — это обработчик прерывания для USART2,
// который вызывается, когда данные поступают в USART2.

// USART_SR(USART2) &= ~(USART_SR_RXNE); — сбрасывает флаг RXNE (Receive Not Empty), который указывает, что в регистре приема есть данные. 
// Это важно для предотвращения повторной обработки того же события.
// usart_recv(USART2) — функция, которая получает данные из USART2. 
// Данные преобразуются в тип uint8_t и добавляются в кольцевой буфер, если он не полон (buf.not_full()).
// gpio_toggle(GPIOE, GPIO11); — переключает состояние светодиода, подключенного к порту GPIOE, пину GPIO11. 
// Это может использоваться для индикации поступления данных.

void usart2_exti26_isr(void){

    USART_SR(USART2) &= ~(USART_SR_RXNE);

    read_data_UART();
    for (volatile uint32_t i = 0; i < 20000; i++); 
    gpio_toggle (GPIOD, GPIO12);


}  
 
void read_data_UART(void){

    uint8_t data = static_cast<uint8_t>(usart_recv (USART2));
    buf.put(data);
     usart_send_blocking(USART2, data );
    gpio_toggle (GPIOD, GPIO14);


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
    usart_set_baudrate(USART2, 115200); // Устанавливаем скорость бит/с 
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


void setup_LED(void){
    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15| GPIO14|  GPIO13| GPIO12);
    // gpio_set(GPIOD, GPIO15|GPIO14|GPIO13|GPIO12);
}



void led_blink_15(uint32_t tome) {


      gpio_set(GPIOD, GPIO15);
        for (volatile uint32_t i = 0; i < tome*1000; i++);
      gpio_clear(GPIOD, GPIO15); 
        for (volatile uint32_t i = 0; i < tome*1000; i++); 
             // Задержка

}

void led_blink_14(uint32_t tome) {


     gpio_set(GPIOD, GPIO14);
        for (volatile uint32_t i = 0; i < tome*1000; i++);   
      gpio_clear(GPIOD, GPIO14); 
        for (volatile uint32_t i = 0; i < tome*1000; i++); 
    

}

void led_blink_13(uint32_t tome) {

      gpio_set(GPIOD, GPIO13);
        for (volatile uint32_t i = 0; i < tome*1000; i++);  
      gpio_clear(GPIOD, GPIO13); 
        for (volatile uint32_t i = 0; i < tome*1000; i++); 


}


void led_blink_12(uint32_t tome) {


    gpio_set(GPIOD, GPIO12);
        for (volatile uint32_t i = 0; i < tome*1000; i++);   

    gpio_clear(GPIOD, GPIO12); 
        for (volatile uint32_t i = 0; i < tome*1000; i++); 
       
    
}




int main(void)  {

    uart_setup();
    setup_LED();
    read_data_UART();
   // usart_transmit();
    

    while (1) {


     //loop();
     usart2_exti26_isr();
     // led_blink_15(1500);
      //led_blink_14(750);
    //  led_blink_13(500);
    //  led_blink_12(1000);
         
    }
}



