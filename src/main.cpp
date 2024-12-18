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

// void usart2_read_and_send_in_usart3(void) {
//     const int BUFFER_SIZE = 126;
//     static char data[BUFFER_SIZE];
//     static int index = 0; // Сделаем индекс статическим, чтобы сохранять его между вызовами
//     char received_char;

//     // Проверяем, есть ли данные для чтения
//     while (usart_get_flag(USART2, USART_SR_RXNE)) {
//         received_char = usart_recv(USART2); // Читаем символ из USART2

//         // Проверяем, получен ли символ конца строки
//         if (received_char == '\n' || received_char == '\r') {
//             // Завершаем строку нулевым символом
//             data[index] = '\0';

//             // Отправляем строку в USART3 с символами начала и конца строки
//             usart_send_blocking(USART3, '\n');
//             for (int i = 0; i < index; i++) {
//                 usart_send_blocking(USART3, data[i]);
//             }
//             usart_send_blocking(USART3, '\r');

//             // Сбрасываем индекс для следующей строки
//             index = 0;
//         } else if (index < BUFFER_SIZE - 1) { // Проверяем, не переполнен ли буфер
//             data[index++] = received_char; // Сохраняем полученный символ в массив
//         }

//         gpio_toggle(GPIOD, GPIO15); // Индикатор получения данных
//     }

//     // Если данные не были получены
//     if (index == 0) {
//         usart_send_blocking(USART3, 'N');
//         usart_send_blocking(USART3, 'o');
//         usart_send_blocking(USART3, ' ');
//         usart_send_blocking(USART3, 'd');
//         usart_send_blocking(USART3, 'a');
//         usart_send_blocking(USART3, 't');
//         usart_send_blocking(USART3, 'a');
//         usart_send_blocking(USART3, ' ');
//         usart_send_blocking(USART3, '\n');
//         gpio_toggle(GPIOD, GPIO14); // Индикатор отсутствия данных
//     }
// }

void usart2_read_and_send_in_usart3(void) {
    const int BUFFER_SIZE = 126;
     static int index = 0;
     char data[BUFFER_SIZE];  
      
    char received_char;      

    if(usart_get_flag(USART2,USART_SR_RXNE)) {

    
          // Оставляем место для завершающего нуля
        received_char = usart_recv(USART2); // Читаем символ из USART2


        // data[index++] = received_char; // Увеличиваем индекс после записи

       

        //   data[index] = '\0';
         

    //  usart_send_blocking(USART3, '\n');

    // for (int i = 0; i < index; i++) {
        // usart_send_blocking(USART3, data[i]);
         usart_send_blocking(USART3, received_char);

    // }
    // usart_send_blocking(USART3, '\r'); 


      gpio_toggle(GPIOD, GPIO15);
    }


    else{

         gpio_toggle(GPIOD, GPIO14);
    // usart_send_blocking(USART3, 'N');
    // usart_send_blocking(USART3, 'o');
    // usart_send_blocking(USART3, ' ');
    // usart_send_blocking(USART3, 'd');
    // usart_send_blocking(USART3, 'a');
    // usart_send_blocking(USART3, 't');
    // usart_send_blocking(USART3, 'a');
    // usart_send_blocking(USART3, ' ');
    // usart_send_blocking(USART3, '\n');

            // data[index] = '\0';

    // Отправляем строку в USART3 с символами начала и конца строки
    // usart_send_blocking(USART3, '\n');
    // for (int i = 0; i < index; i++) {
    //     usart_send_blocking(USART3, data[i]);
    // }
    // usart_send_blocking(USART3, '\r'); 


   
    }
       
}

int main(void) {
    usart2_setup(); // Настройка USART2
    usart3_setup(); // Настройка USART3
    setup_LED(); // Настройка LED

    // usart_send_blocking(USART3, 'T');
    // usart_send_blocking(USART3, 'E');
    // usart_send_blocking(USART3, 'S');
    // usart_send_blocking(USART3, 'T');

    while (1) {
        usart2_read_and_send_in_usart3(); // Обработка данных
        // Включаем светодиод, если данные не получены
    //for (volatile uint32_t i = 0; i < 500000; i++); // Задержка
    }
}



// создай массив который имеет две стороны , первая сторона имеется запись данных а с другйо стороны чтение данных . нужно два индекса wr и rd . вначале работы инксы этих массивов равны 0 , 
// когда мы что-то записываем в ячейку (wr) его индекс сдвинули на 1 при это читаем из уже записанного , а когда причиталаи здвигаем индекс на 1. И так пока массив с принятыми данными не заполнится 