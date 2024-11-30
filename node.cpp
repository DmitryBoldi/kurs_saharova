

// #include <libopencm3/stm32/rcc.h>
// #include <libopencm3/stm32/gpio.h>
// #include <libopencm3/stm32/usart.h>
// #include <libopencm3/cm3/nvic.h>
// #include <stdio.h>
// #include <string.h>

// #define BUFFER_SIZE 256

// void usart3_setup(void);
// void usart_send_string(const char *str);
// void usart_receive_data(void);
// void parse_nmea_sentence(const char *sentence);

// char buffer[BUFFER_SIZE];
// uint32_t buffer_index = 0;

// void usart3_setup(void) {
//     // Включаем тактирование для GPIOB и USART3
//     rcc_periph_clock_enable(RCC_GPIOB);
//     rcc_periph_clock_enable(RCC_USART3);

//     // Настраиваем пины PB10 (TX) и PB11 (RX) для USART3
//     gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO10); // TX
//     gpio_set_af(GPIOB, GPIO_AF7, GPIO11); // RX

//     // Настраиваем USART3
//     usart_set_baudrate(USART3, 115200); // Устанавливаем скорость бит/с 
//     usart_set_databits(USART3, 8); // 8 бит данных
//     usart_set_stopbits(USART3, USART_STOPBITS_1); // 1 стоп-бит
//     usart_set_mode(USART3, USART_MODE_TX_RX); // Режим TX и RX
//     usart_set_parity(USART3, USART_PARITY_NONE); // Без четности
//     usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE); // Без управления потоком

//     usart_enable_rx_interrupt(USART3);
//     nvic_enable_irq(NVIC_USART3_IRQ); // Включаем прерывание для USART3

//     // Включаем USART3
//     usart_enable(USART3);
// }

// void usart_send_string(const char *str) {
//     while (*str) {
//         usart_send(USART3, *str++); // Отправка каждого символа
//     }
// }

// void usart_receive_data(void) {
//     // Функция будет обрабатывать данные в прерывании
// }

// void parse_nmea_sentence(const char *sentence) {
//     // Проверка на наличие нужных типов сообщений (GPGGA, GPRMC и т.д.)
//     if (strstr(sentence, "GPGGA") != NULL) {
//         usart_send_string("Received GPGGA sentence:\n");
//         usart_send_string(sentence);
//         usart_send_string("\n");
//     }
// }

// // Обработчик прерывания для USART3
// void usart3_isr(void) {
//     char c;
//     if (usart_get_flag(USART3, USART_SR_RXNE)) { // Проверяем, есть ли данные для чтения
//         c = usart_recv(USART3); // Получаем символ
//         if (c == '$') {
//             buffer_index = 0; // Сбрасываем индекс при начале новой строки
//         }
//         if (buffer_index < BUFFER_SIZE - 1) {
//             buffer[buffer_index++] = c; // Сохраняем символ в буфер
//             buffer[buffer_index] = '\0'; // Обеспечиваем нуль-терминатор
//         }
//         if (c == '\n') {
//             parse_nmea_sentence(buffer); // Парсим NMEA строку
//             buffer_index = 0; // Сбрасываем индекс после обработки
//         }
//     }
// }

// int main(void) {
//     usart3_setup(); // Настройка USART3

    
//     // Входим в бесконечный цикл ожидания данных
//     while (1) {
//     // usart_send_string("GPS/GLONASS NMEA Parser Initialized");
//     // for (volatile uint32_t i = 0; i < 500000; i++);
//     usart3_isr();
//         // Основной цикл может быть пустым, все происходит в прерывании
//     }

//     return 0;
// }
