// Библиотека <cstdint> предоставляет фиксированные по размеру целочисленные типы,
// такие как uint8_t, который представляет собой беззнаковый 8-битный целое число.
#include <cstdint>

class Ring_buffer{
// #define CAPACITY 128

public: 
//Статическая константа , для определения ёмкости буфера которая равна 128 элментов 
    static constexpr uint8_t CAPACITY = 255;
//Конструктор      
    Ring_buffer();
//Функция для получения данных
    uint8_t get();
//Метод для добавления элементов , который принимает 8 битные безнаковые значения 
    void put(uint8_t);
//Метод для проверки буферка, пуст ли буффер 
    bool empty();
//МЕтод для проверкм заполнен ли буффер 
    bool not_full();
    bool full();
private:

  private:
    uint8_t wr_idx; // Индекс записи (где будет записан следующий элемент)
    uint8_t rd_idx; // Индекс чтения (откуда будет прочитан следующий элемент)
    uint8_t size;   // Текущий размер буфера (количество элементов в буфере)
    uint8_t capacity{CAPACITY}; // Вместимость буфера
    uint8_t arr[CAPACITY]; // Массив для хранения элементов буфера

};
