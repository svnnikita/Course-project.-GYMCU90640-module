#define DATA_SIZE 1544  // размер принятого кадра

// структура для хранения необработанных данных с датчика
typedef struct sensor_data {
    uint8_t frame_buffer[DATA_SIZE];        // буффер для хранения кадра данных с датчика
    bool frame_ready;                       // флаг принятия кадра
} sensor_data;

// Тактирование
void Clock_Setup(void);

// Настройка UART для обмена данными с датчиком
void UART2_GYMCU90640_Setup(void);

// Настраиваем UART, по которому будем видеть обработанные данные с датчика
void UART1_Data_Setup(void);

// Отправляем строку символов через интерфейс USART в блокирующем режиме
void UART_Send_Command(uint8_t *command);

// Обработка данных с датчика
void UART2_Processing(sensor_data *sensor);