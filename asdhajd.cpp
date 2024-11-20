#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h> //заголовок для перерывааний находиться в другой папке 
#include <libopencm3/stm32/usart.h>
void setup()
{
// Интерфейс U(S)ART с внешним миром
rcc_periph_clock_enable(RCC_GPIOA);                           // Разморозка порта ввода/вывода

/заголовок для перерывааний находиться в другой папке 
gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);  // Режим альтернативной функции
gpio_set_af(GPIOA,GPIO_AF7, GPIO9 | GPIO10);                           // Альтернативная функция (выбор по номеру) PA9 --- Tx, PA10 --- Rx.

constexpr uint16_t PERIOD_MS{1000};

void blink_LED() 
{
if(timer_get_counter(TIM1) < PERIOD_MS / 2)   
  {
    gpio_set(GPIOE, GPIO9);  
  } 
// Настройка и включение U(S)ART, номер устройства настраивается
// для stm32f303 --- в диапазоне 1 -- 5; 4 и 5 --- UART.
// для stm32f407 --- в диапазоне 1 -- 6; 4 и 5 --- UART.

rcc_periph_clock_enable(RCC_USART1);                      // Разморозка ПУ

else 
  {
  gpio_clear(GPIOE, GPIO9);
  } 
usart_set_baudrate(USART1, 115200);                       // Скорость передачи
usart_set_databits(USART1, 8);                            // Размер посылки
usart_set_stopbits(USART1, USART_STOPBITS_1);             // Количество стоп-битов
usart_set_parity(USART1, USART_PARITY_NONE);              // Контроль четности

}
usart_set_mode(USART1, USART_MODE_TX_RX);                 // Режим работы ПУ
usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);   // Управление процессом передачи сообщений

usart_enable(USART1);                                     // Включение ПУ

void LED_gpio_setup()
}
void loop()
{
  rcc_periph_clock_enable( RCC_GPIOE);
  gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9 | GPIO11  | GPIO14 );

  gpio_mode_setup(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO14 );

  gpio_set_af(GPIOE, GPIO_AF2, GPIO14);
usart_send_blocking(USART1, 0x55);
usart_send_blocking(USART1, 'H');
usart_send_blocking(USART1, 'e');
usart_send_blocking(USART1, 'l');
usart_send_blocking(USART1, 'l');
usart_send_blocking(USART1, 'o');
usart_send_blocking(USART1, '\n');
usart_send_blocking(USART1, '\r');
for(volatile uint32_t i=0 ; i<1000; i++); 

}

void timer_setup()
int main()
{
  rcc_periph_clock_enable( RCC_TIM1);
  timer_set_prescaler (TIM1, rcc_get_timer_clk_freq(TIM1) / PERIOD_MS -1  );
  timer_set_period(TIM1, PERIOD_MS-1);
//для разрешения беспокойства процессора 
  timer_enable_irq(TIM1, TIM_DIER_UIE);
  nvic_enable_irq (NVIC_TIM1_UP_TIM16_IRQ ); 
  setup();

  timer_enable_counter(TIM1);
  while(true)
  {
    loop();
  }
}


LED_gpio_setup();
timer_setup();


  while (true )
 {
  blink_LED();


  }






void tim1_up_tim16_isr(void)  //обработчик  
{

timer_clear_flag(TIM1, TIM_SR_UIF); // снимаем заброс или выключаем будильник.  TIM_SR_UIF- для обнуленяи
//полезная работа те переключение сетодиода  
gpio_toggle(GPIOE, GPIO11) ;
}


