#include <Arduino.h>
#include "Serial9b.h"
//#include <HardwareSerial_private.h>
#include "Serial9b_private.h"

#if defined(HAVE_HWSERIAL2)

HardwareSerial9b Serial9b2(&UBRR2H, &UBRR2L, &UCSR2A, &UCSR2B, &UCSR2C, &UDR2);

// TODO: Need to prevent instantiation of HardwareSerial1.cpp
#if defined(UART2_RX_vect)
ISR(UART2_RX_vect)
#elif defined(USART2_RX_vect)
ISR(USART2_RX_vect)
#else
#error "Don't know what the Data Register Empty vector is called for Serial1"
#endif
{
  Serial9b2._rx_complete_irq_9bit();
}

#if defined(UART2_UDRE_vect)
ISR(UART2_UDRE_vect)
#elif defined(USART2_UDRE_vect)
ISR(USART2_UDRE_vect)
#else
#error "Don't know what the Data Register Empty vector is called for Serial1"
#endif
{
  Serial9b2._tx_udr_empty_irq_9bit();
}

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial9b2_available() {
  return Serial9b2.available();
}


#endif
