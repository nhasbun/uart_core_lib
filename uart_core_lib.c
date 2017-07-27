#include <stdio.h> // printf setvbuf
#include <math.h> // round
#include <string.h> // strlen
#include "socal/socal.h" // alt_write_hword alt_read_hword
#include "socal/hps.h" // para BASEADD
#include "uart_core_lib.h"

#define BIT(x,n) (((x) >> (n)) & 1)

UART_T uart_init(uint32_t baseadd)
{
  UART_T uart;
  uart.BASEADD   = baseadd;
  uart.RXDATA    = baseadd + (uint32_t)(0x00);
  uart.TXDATA    = baseadd + (uint32_t)(0x04);
  uart.STATUS    = baseadd + (uint32_t)(0x08);
  uart.CONTROL   = baseadd + (uint32_t)(0x0c);
  uart.DIVISOR   = baseadd + (uint32_t)(0x10);
  uart.ENDOFPACK = baseadd + (uint32_t)(0x14);

  return uart;
}

uint16_t printUartAdd(uint32_t address)
{
  uint16_t res = alt_read_hword(address);
  printf("%i\n", res);

  return res;
}


void setBaud(UART_T * uart_p, uint32_t clock_frequency, uint32_t baudrate)
{
  // Formula segun
  // Embedded Peripherals IP User Guide -  UART Core
  float divisor = round((float)clock_frequency/(float)baudrate + 0.5f);
  alt_write_hword(uart_p->DIVISOR, (uint32_t)divisor);
}

void checkUartRegisters(UART_T * uart_p)
{
  printf("%s ", "rxdata:");
  printUartAdd(uart_p->RXDATA);
  printf("%s ", "txdata:");
  printUartAdd(uart_p->TXDATA);
  printf("%s ", "status:");
  printUartAdd(uart_p->STATUS);
  printf("%s ", "control:");
  printUartAdd(uart_p->CONTROL);
  printf("%s ", "divisor:");
  printUartAdd(uart_p->DIVISOR);
  printf("%s ", "end of packet:");
  printUartAdd(uart_p->ENDOFPACK);
}

void resetStatus(UART_T * uart_p)
{
  alt_write_hword(uart_p->STATUS, 0);
}

// Checkea si hay algun dato en espera a ser leido
// true si hay un dato en espera
bool check_rxdata(UART_T * uart_p)
{
  uint16_t status = alt_read_hword(uart_p->STATUS);
  return BIT(status, 7);
}

// Checkea si el registro esta listo para enviar nuevo caracter
// true si esta listo
bool check_txdata(UART_T * uart_p)
{
  uint16_t status = alt_read_hword(uart_p->STATUS);
  return BIT(status, 6);
}

void enviarchar(UART_T * uart_p, char character)
{
  while(check_txdata(uart_p) == 0) {}; // txready es 1 cuando
  // la transmision anterios se completo
  alt_write_hword(uart_p->TXDATA, (uint16_t)character);
}

void enviarstring(UART_T * uart_p, char * str, int num)
{
  for (int i = 0; i < num; ++i) {
    enviarchar(uart_p, *str);
    str++;
  }
}

char recibirchar(UART_T * uart_p)
{
  return (char)alt_read_hword(uart_p->RXDATA);
}