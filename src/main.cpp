#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <stdio.h>
#include <cstring>

// ИНИЦИЛИЗАЦИЯ ВСЕХ УСТРОЙСТВ ДЛЯ РАБОТЫ

void usart2_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART2);
    
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2); // TX
    gpio_set_af(GPIOA, GPIO_AF7, GPIO3); // RX

    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    nvic_enable_irq(NVIC_USART2_IRQ);
    usart_enable(USART2);
}

void usart3_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_USART3);
    
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
    gpio_set_af(GPIOB, GPIO_AF7, GPIO10); // TX
    gpio_set_af(GPIOB, GPIO_AF7, GPIO11); // RX

    usart_set_baudrate(USART3, 115200);
    usart_set_databits(USART3, 8);
    usart_set_stopbits(USART3, USART_STOPBITS_1);
    usart_set_mode(USART3, USART_MODE_TX_RX);
    usart_set_parity(USART3, USART_PARITY_NONE);
    usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);

    nvic_enable_irq(NVIC_USART3_IRQ);
    usart_enable(USART3);
}

void setup_LED(void) {
    rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15 | GPIO14 | GPIO13 | GPIO12);
}

// ОСНОВНЫЕ ФУНКЦИИ В КОТОРЫХ ПРОИСХОДИТ ВСЯ МАГИЯ
void usart2_read_and_send_in_usart3(void) {
    const int BUFFER_SIZE = 126;
    char data[BUFFER_SIZE];  
    int index = 0;       
    char received_char;      

    while (index < BUFFER_SIZE - 1) {  // Оставляем место для завершающего нуля
        received_char = usart_recv(USART2); // Читаем символ из USART2

        // Проверяем, получен ли символ конца строки
        if (received_char == '\n' || received_char == '\r') {
            break; // Выход из цикла при получении конца строки
        }

        // Сохраняем полученный символ в массив
        data[index++] = received_char; // Увеличиваем индекс после записи
    }

    // Завершаем строку нулевым символом
    data[index] = '\0';

    // Отправляем строку в USART3 с символами начала и конца строки
    usart_send_blocking(USART3, '\n');
    for (int i = 0; i < index; i++) {
        usart_send_blocking(USART3, data[i]);
    }
    usart_send_blocking(USART3, '\r'); 
    gpio_toggle(GPIOD, GPIO15);
}

int main(void) {
    usart2_setup(); // Настройка USART2
    usart3_setup(); // Настройка USART3
    setup_LED(); // Настройка LED

    usart_send_blocking(USART3, 'T');
    usart_send_blocking(USART3, 'E');
    usart_send_blocking(USART3, 'S');
    usart_send_blocking(USART3, 'T');

    while (1) {
        usart2_read_and_send_in_usart3(); // Обработка данных
        // Включаем светодиод, если данные не получены
        gpio_toggle(GPIOD, GPIO13);
        for (volatile uint32_t i = 0; i < 500000; i++); // Задержка
    }
}


// #include <libopencm3/stm32/rcc.h>
// #include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/usart.h>
// #include <libopencm3/cm3/nvic.h>
// #include <stdio.h>
// #include <cstring>

// // ИНИЦИЛИЗАЦИЯ ВСЕХ УСТРОЙСТВ ДЛЯ РАБОТЫ

// void usart2_setup(void) {
//     rcc_periph_clock_enable(RCC_GPIOA);
//     rcc_periph_clock_enable(RCC_USART2);
    
//     gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO2); // TX
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO3); // RX

//     usart_set_baudrate(USART2, 115200);
//     usart_set_databits(USART2, 8);
//     usart_set_stopbits(USART2, USART_STOPBITS_1);
//     usart_set_mode(USART2, USART_MODE_TX_RX);
//     usart_set_parity(USART2, USART_PARITY_NONE);
//     usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

//     nvic_enable_irq(NVIC_USART2_IRQ);
//     usart_enable(USART2);
// }

// void usart3_setup(void) {
//     rcc_periph_clock_enable(RCC_GPIOB);
//     rcc_periph_clock_enable(RCC_USART3);
    
//     gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO10); // TX
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO11); // RX

//     usart_set_baudrate(USART3, 115200);
//     usart_set_databits(USART3, 8);
//     usart_set_stopbits(USART3, USART_STOPBITS_1);
//     usart_set_mode(USART3, USART_MODE_TX_RX);
//     usart_set_parity(USART3, USART_PARITY_NONE);
//     usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);

//     nvic_enable_irq(NVIC_USART3_IRQ);
//     usart_enable(USART3);
// }

// void setup_LED(void) {
//     rcc_periph_clock_enable(RCC_GPIOD);
//     gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15 | GPIO14 | GPIO13 | GPIO12);
// }

// // ОСНОВНЫЕ ФУНКЦИИ В КОТОРЫХ ПРОИСХОДИТ ВСЯ МАГИЯ
// void usart2_read_and_send_in_usart3(void) {
//     const int BUFFER_SIZE = 126;
//     char data[BUFFER_SIZE];  
//     int index = 0;       
//     char received_char;      

//     while (index < BUFFER_SIZE) {
//         received_char = usart_recv(USART2); // Читаем символ из USART2

//         // // Проверяем, получен ли символ конца строки
//         // if (received_char == '\n' || received_char == '\r') {
//         //     break; // Выход из цикла при получении конца строки
//         // }

//         // Сохраняем полученный символ в массив
//         data[index++] = received_char; // Увеличиваем индекс после записи

//         // Отправляем сразу полученный символ в USART3
//      usart_send_blocking(USART3, '\n');
//         usart_send_blocking(USART3, received_char); 
//         usart_send_blocking(USART3, '\r'); 
//         gpio_toggle(GPIOD, GPIO15);


//     //     if (index >= BUFFER_SIZE) {
//     //     memset(data, 0, sizeof(data)); // Обнуляем массив
//     //     usart_send_blocking(USART3, 'M'); // Отправляем сообщение о переполнении
//     //     usart_send_blocking(USART3, 'A');
//     //     usart_send_blocking(USART3, 'S');
//     //     usart_send_blocking(USART3, 'S');
//     //     usart_send_blocking(USART3, 'I');
//     //     usart_send_blocking(USART3, 'V');
//     //     usart_send_blocking(USART3, 'E');
//     //     usart_send_blocking(USART3, ' '); // Отправляем сообщение о переполнении
//     //     usart_send_blocking(USART3, 'F');
//     //     usart_send_blocking(USART3, 'U');
//     //     usart_send_blocking(USART3, 'L');
//     //     usart_send_blocking(USART3, 'L');
//     //     gpio_toggle(GPIOD, GPIO14);
//     // }
//     }

//     // Если массив полон, обнуляем его и отправляем сообщение об этом
    
// }

// int main(void) {
//     usart2_setup(); // Настройка USART2
//     usart3_setup(); // Настройка USART3
//     setup_LED(); // Настройка LED

//     usart_send_blocking(USART3, 'T');
//     usart_send_blocking(USART3, 'E');
//     usart_send_blocking(USART3, 'S');
//     usart_send_blocking(USART3, 'T');

//     while (1) {
//         usart2_read_and_send_in_usart3(); // Обработка данных
//         // Включаем светодиод, если данные не получены
//         gpio_toggle(GPIOD, GPIO13);
//         for (volatile uint32_t i = 0; i < 500000; i++); // Задержка
//     }
// }










// #include <libopencm3/stm32/rcc.h>
// #include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/usart.h>
// #include <libopencm3/cm3/nvic.h>
// #include <stdio.h>

// #define BUFFER_SIZE 10

// void read_data_UART(uint8_t *size_buffer, uint32_t *index);
// void usart_send_string(const char *str);
// void usart_send_test(void);

// void read_data_UART(uint8_t *size_buffer, uint32_t *index) {
//     if (*index < BUFFER_SIZE) { // Проверяем, не превышает ли индекс размер буфера
//         size_buffer[*index] = usart_recv(USART2); // Читаем байт из USART
        
//         (*index)++; // Увеличиваем индекс

//         gpio_toggle(GPIOD, GPIO14);
//         for (volatile uint32_t i = 0; i < 500000; i++); // Задержка
//     } else {
//         // Обработка переполнения буфера
//         char error_message[30];
//         snprintf(error_message, sizeof(error_message), "  Error: Buffer full");
//         usart_send_string(error_message);
//         gpio_set(GPIOD, GPIO15);
//         for (volatile uint32_t i = 0; i < 500000; i++); // Задержка
//     }
// }

// // Функция для отправки строки по USART
// void usart_send_string(const char *str) {
//     while (*str) {
//         usart_send(USART3, *str++); // Отправка каждого символа
//         for (volatile uint32_t i = 0; i < 500000; i++); // Задержка
//     }
// }

// void usart_send_buffer(uint8_t *buffer, uint32_t size) {
//     for (uint32_t i = 0; i < size; i++) { // Используем size вместо BUFFER_SIZE
//         usart_send(USART3, buffer[i]); // Отправка каждого байта
//         gpio_toggle(GPIOD, GPIO12);
//         for (volatile uint32_t i = 0; i < 500000; i++); // Задержка
//     }
// }

// void usart2_setup(void) {
//     rcc_periph_clock_enable(RCC_GPIOA);
//     rcc_periph_clock_enable(RCC_USART2);
    
//     gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO2); // TX
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO3); // RX

//     usart_set_baudrate(USART2, 115200);
//     usart_set_databits(USART2, 8);
//     usart_set_stopbits(USART2, USART_STOPBITS_1);
//     usart_set_mode(USART2, USART_MODE_TX_RX);
//     usart_set_parity(USART2, USART_PARITY_NONE);
//     usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

//     nvic_enable_irq(NVIC_USART2_IRQ);
//     usart_enable(USART2);
// }

// void usart3_setup(void) {
//     rcc_periph_clock_enable(RCC_GPIOB);
//     rcc_periph_clock_enable(RCC_USART3);
    
//     gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO10); // TX
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO11); // RX

//     usart_set_baudrate(USART3, 115200);
//     usart_set_databits(USART3, 8);
//     usart_set_stopbits(USART3, USART_STOPBITS_1);
//     usart_set_mode(USART3, USART_MODE_TX_RX);
//     usart_set_parity(USART3, USART_PARITY_NONE);
//     usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);

//     nvic_enable_irq(NVIC_USART3_IRQ);
//     usart_enable(USART3);
// }

// void setup_LED(void) {
//     rcc_periph_clock_enable(RCC_GPIOD);
//     gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15 | GPIO14 | GPIO13 | GPIO12);
// }

// void usart_send_test(void) {
//     usart_send_string("Hello, USART!\n");
// }

// int main(void) {
//     uint8_t size_buffer[BUFFER_SIZE];
//     uint32_t index = 0;

//     usart2_setup(); // Настройка USART2
//     usart3_setup(); // Настройка USART3
//     setup_LED(); // Настройка LED

//     usart_send_test(); // Отправка тестового сообщения

//     while (1) {
//         read_data_UART(size_buffer, &index); // Чтение данных из USART
//         usart_send_buffer(size_buffer, index); // Отправка буфера
//         gpio_set(GPIOD, GPIO13); // Индикатор работы
//     }
// }

// #include <libopencm3/stm32/rcc.h>
// #include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/usart.h>
// #include <libopencm3/cm3/nvic.h>
// #include <stdio.h>

// #define BUFFER_SIZE 255

// void read_data_UART(uint8_t *size_buffer, uint32_t *index);
// void usart_send_string(const char *str);

// void read_data_UART(uint8_t *size_buffer, uint32_t *index) {
//     char number_buffer[30]; // Увеличен размер буфера для сообщений

//     if (*index < BUFFER_SIZE) { // Проверяем, не превышает ли индекс размер буфера
//         size_buffer[*index] = usart_recv(USART2); // Читаем байт из USART
//         (*index)++; // Увеличиваем индекс


//       gpio_toggle(GPIOD, GPIO14);
//       for (volatile uint32_t i = 0; i < 500000; i++);
   
   

//     } else {
//         // Обработка переполнения буфера
//         snprintf(number_buffer, sizeof(number_buffer), "Error: Buffer full  ");
//         usart_send_string(number_buffer);
//         gpio_set(GPIOD,GPIO15);
//         for (volatile uint32_t i = 0; i < 500000; i++);
//         // break;
        
      
//     }
// }

// // Функция для отправки строки по USART
// void usart_send_string(const char *str) {
//     while (*str) {
//         usart_send(USART3, *str++); // Отправка каждого символа
//         for (volatile uint32_t i = 0; i < 500000; i++);
//     }
// }

// void usart_send_buffer(uint8_t *buffer, uint32_t size) {
//     for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
//         usart_send(USART3, buffer[i]); // Отправка каждого байта
//         gpio_toggle(GPIOD,GPIO12);
//         for (volatile uint32_t i = 0; i < 500000; i++);
       

//     }
// }

// void usart2_setup(void) {
//     // Включаем тактирование для GPIOA и USART2
//     rcc_periph_clock_enable(RCC_GPIOA);
//     rcc_periph_clock_enable(RCC_USART2);
    
//     // Настраиваем пины PA2 (TX) и PA3 (RX) для USART2
//     gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO2); // TX
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO3); // RX

//     // Настраиваем USART2
//     usart_set_baudrate(USART2, 9600); // Устанавливаем скорость бит/с 
//     usart_set_databits(USART2, 8); // 8 бит данных
//     usart_set_stopbits(USART2, USART_STOPBITS_1); // 1 стоп-бит
//     usart_set_mode(USART2, USART_MODE_TX_RX); // Режим TX и RX
//     usart_set_parity(USART2, USART_PARITY_NONE); // Без четности
//     usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE); // Без управления потоком

//     //usart_enable_rx_interrupt(USART2);
//     nvic_enable_irq(NVIC_USART2_IRQ); // Включаем прерывание для USART2

//     // Включаем USART2
//     usart_enable(USART2);
// }


// void usart3_setup(void) {
//  // Включаем тактирование для GPIOB и USART3
//     rcc_periph_clock_enable(RCC_GPIOB);
//     rcc_periph_clock_enable(RCC_USART3);
    


//       // Настраиваем пины PB10 (TX) и PB11 (RX) для USART2
//     gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO10); // TX
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO11); // RX

//     // Настраиваем USART3
//     usart_set_baudrate(USART3, 9600); // Устанавливаем скорость бит/с 
//     usart_set_databits(USART3, 8); // 8 бит данных
//     usart_set_stopbits(USART3, USART_STOPBITS_1); // 1 стоп-бит
//     usart_set_mode(USART3, USART_MODE_TX_RX); // Режим TX и RX
//     usart_set_parity(USART3, USART_PARITY_NONE); // Без четности
//     usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE); // Без управления потоком

//    //usart_enable_rx_interrupt(USART3);
//     nvic_enable_irq(NVIC_USART3_IRQ); // Включаем прерывание для USART2

//     // Включаем USART3
//     usart_enable(USART3);
// }


// void setup_LED(void) {
//     rcc_periph_clock_enable(RCC_GPIOD);
//     gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15 | GPIO14 | GPIO13 | GPIO12);
// }



// int main(void) {
//     uint8_t size_buffer[BUFFER_SIZE]; // Изменяем тип на uint8_t
//     uint32_t index = 0;
//     int i=0 ;

//     usart2_setup(); // Настройка USART2
//     usart3_setup();
//     setup_LED(); // Настройка LED

//    for(int i=0;i<30;i++) {
//         read_data_UART(size_buffer, &index); // Передаем указатели на массив и индекс
//         usart_send_buffer(size_buffer, index);
//        gpio_set(GPIOD,GPIO13);
      

//     }
// }