#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h> // open

#define BIT(x,n) (((x) >> (n)) & 1)

typedef struct UART_S UART_T;

struct UART_S {
  uint32_t BASEADD;
  uint32_t RXDATA;
  uint32_t TXDATA;
  uint32_t STATUS;
  uint32_t CONTROL;
  uint32_t DIVISOR;
  uint32_t ENDOFPACK;

  // UART buffer for incoming data
  uint32_t  buffer_size;
  uint8_t * buffer_pointer_init;
  uint8_t * buffer_pointer_writing;
  uint8_t * buffer_pointer_reading;
  uint32_t  buffer_count;
  
  pthread_t thread;
  bool      thread_run;
};

// Initial config and utils
UART_T uart_init(uint32_t baseadd);
void setBaud(UART_T * uart_p, uint32_t clock_frequency, uint32_t baudrate);
void setBufferSize(UART_T * uart_p, uint32_t size);
void freeBuffer(UART_T * uart_p);

// UART Core registers access
uint16_t uartRegister(uint32_t address);
void checkUartRegisters(UART_T * uart_p);
void resetStatus(UART_T * uart_p);

// proceso de envio de datos
bool checkTxdata(UART_T * uart_p);
void enviarChar(UART_T * uart_p, char character);
void enviarString(UART_T * uart_p, char * str, int num);

// proceso de recepcion de datos
bool checkRxdata(UART_T * uart_p);
uint8_t recibirChar(UART_T * uart_p);

// proceso de recepcion automatica de datos
void activarRecepcion(UART_T * uart_p);
void * activarRecepcion_aux(void * ptr);
void terminarRecepcion(UART_T * uart_p);
uint8_t * vaciarBuffer(UART_T * uart_p, uint32_t * num);
