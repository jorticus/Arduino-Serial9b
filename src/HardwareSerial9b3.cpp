#include <Arduino.h>
#include "Serial9b.h"
//#include <HardwareSerial_private.h>
#include "Serial9b_private.h"

#if defined(HAVE_HWSERIAL3)

HardwareSerial9b Serial9b3(&UBRR3H, &UBRR3L, &UCSR3A, &UCSR3B, &UCSR3C, &UDR3);

// TODO: Need to prevent instantiation of HardwareSerial1.cpp
#if defined(UART3_RX_vect)
ISR(UART3_RX_vect)
#elif defined(USART3_RX_vect)
ISR(USART3_RX_vect)
#else
#error "Don't know what the Data Register Empty vector is called for Serial1"
#endif
{
  Serial9b3._rx_complete_irq_9bit();
}

#if defined(UART3_UDRE_vect)
ISR(UART3_UDRE_vect)
#elif defined(USART3_UDRE_vect)
ISR(USART3_UDRE_vect)
#else
#error "Don't know what the Data Register Empty vector is called for Serial1"
#endif
{
  Serial9b3._tx_udr_empty_irq_9bit();
}

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial9b3_available() {
  return Serial9b3.available();
}


#endif
