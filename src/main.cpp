#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <stdio.h>

#define BUFFER_SIZE 255

void read_data_UART(uint8_t *size_buffer, uint32_t *index);
void usart_send_string(const char *str);

void read_data_UART(uint8_t *size_buffer, uint32_t *index) {
    char number_buffer[30]; // Увеличен размер буфера для сообщений

    if (*index < BUFFER_SIZE) { // Проверяем, не превышает ли индекс размер буфера
        size_buffer[*index] = usart_recv(USART2); // Читаем байт из USART
        (*index)++; // Увеличиваем индекс


      gpio_toggle(GPIOD, GPIO14);
      for (volatile uint32_t i = 0; i < 500000; i++);
   
   

    } else {
        // Обработка переполнения буфера
        snprintf(number_buffer, sizeof(number_buffer), "Error: Buffer full");
        usart_send_string(number_buffer);
        gpio_set(GPIOD,GPIO15);
        for (volatile uint32_t i = 0; i < 500000; i++);
        // break;
        
      
    }
}

// Функция для отправки строки по USART
void usart_send_string(const char *str) {
    while (*str) {
        usart_send(USART3, *str++); // Отправка каждого символа
        for (volatile uint32_t i = 0; i < 500000; i++);
    }
}

void usart_send_buffer(uint8_t *buffer, uint32_t size) {
    for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
        usart_send(USART3, buffer[i]); // Отправка каждого байта
        gpio_toggle(GPIOD,GPIO12);
        for (volatile uint32_t i = 0; i < 500000; i++);
       

    }
}

void usart2_setup(void) {
    // Включаем тактирование для GPIOA и USART2
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2);
    
    // Настраиваем пины PA2 (TX) и PA3 (RX) для USART2
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
    nvic_enable_irq(NVIC_USART2_IRQ); // Включаем прерывание для USART2

    // Включаем USART2
    usart_enable(USART2);
}


void usart3_setup(void) {
 // Включаем тактирование для GPIOB и USART3
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_USART3);
    


      // Настраиваем пины PB10 (TX) и PB11 (RX) для USART2
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
    gpio_set_af(GPIOB, GPIO_AF7, GPIO10); // TX
    gpio_set_af(GPIOB, GPIO_AF7, GPIO11); // RX

    // Настраиваем USART3
    usart_set_baudrate(USART3, 115200); // Устанавливаем скорость бит/с 
    usart_set_databits(USART3, 8); // 8 бит данных
    usart_set_stopbits(USART3, USART_STOPBITS_1); // 1 стоп-бит
    usart_set_mode(USART3, USART_MODE_TX_RX); // Режим TX и RX
    usart_set_parity(USART3, USART_PARITY_NONE); // Без четности
    usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE); // Без управления потоком

    usart_enable_rx_interrupt(USART3);
    nvic_enable_irq(NVIC_USART3_IRQ); // Включаем прерывание для USART2

    // Включаем USART3
    usart_enable(USART3);
}


void setup_LED(void) {
    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15 | GPIO14 | GPIO13 | GPIO12);
}



int main(void) {
    uint8_t size_buffer[BUFFER_SIZE]; // Изменяем тип на uint8_t
    uint32_t index = 0;
    int i=0 ;

    usart2_setup(); // Настройка USART2
    usart3_setup();
    setup_LED(); // Настройка LED

    while (1) {
        read_data_UART(size_buffer, &index); // Передаем указатели на массив и индекс
        usart_send_buffer(size_buffer, index);
       gpio_set(GPIOD,GPIO13);
      

    }
}`