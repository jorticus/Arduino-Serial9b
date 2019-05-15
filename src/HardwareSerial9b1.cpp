#include <Arduino.h>
#include "Serial9b.h"
//#include <HardwareSerial_private.h>
#include "Serial9b_private.h"

#if defined(HAVE_HWSERIAL1)

HardwareSerial9b Serial9b1(&UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1);

// TODO: Need to prevent instantiation of HardwareSerial1.cpp
#if defined(UART1_RX_vect)
ISR(UART1_RX_vect)
#elif defined(USART1_RX_vect)
ISR(USART1_RX_vect)
#else
#error "Don't know what the Data Register Empty vector is called for Serial1"
#endif
{
  Serial9b1._rx_complete_irq_9bit();
}

#if defined(UART1_UDRE_vect)
ISR(UART1_UDRE_vect)
#elif defined(USART1_UDRE_vect)
ISR(USART1_UDRE_vect)
#else
#error "Don't know what the Data Register Empty vector is called for Serial1"
#endif
{
  Serial9b1._tx_udr_empty_irq_9bit();
}

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial9b1_available() {
  return Serial9b1.available();
}


#endif
