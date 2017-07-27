#include <stdbool.h>
#include <fcntl.h> // open

typedef struct UART_S UART_T;

struct UART_S {
  uint32_t BASEADD;
  uint32_t RXDATA;
  uint32_t TXDATA;
  uint32_t STATUS;
  uint32_t CONTROL;
  uint32_t DIVISOR;
  uint32_t ENDOFPACK;
};

UART_T uart_init(uint32_t baseadd);
uint16_t printUartAdd(uint32_t address);
void checkUartRegisters(UART_T * uart_p);
void resetStatus(UART_T * uart_p);
void setBaud(UART_T * uart_p, uint32_t clock_frequency, uint32_t baudrate);

// proceso de envio de datos
bool check_txdata(UART_T * uart_p);
void enviarchar(UART_T * uart_p, char character);
void enviarstring(UART_T * uart_p, char * str, int num);

// proceso de recepcion de datos
bool check_rxdata(UART_T * uart_p);
char recibirchar(UART_T * uart_p);
