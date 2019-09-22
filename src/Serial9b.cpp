
#include "Serial9b.h"
#include "Serial9b_private.h"

HardwareSerial9b::HardwareSerial9b(
    volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
    volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
    volatile uint8_t *ucsrc, volatile uint8_t *udr)
    : HardwareSerial(ubrrh, ubrrl, ucsra, ucsrb, ucsrc, udr)
{ }

HardwareSerial9b::HardwareSerial9b(const HardwareSerial& ser)
    : HardwareSerial(
        0,0,0,0,0,0
        // TODO: This seems to crash the MCU?
        // static_cast<HardwareSerial9b>(ser)._ubrrh,
        // static_cast<HardwareSerial9b>(ser)._ubrrl,
        // static_cast<HardwareSerial9b>(ser)._ucsra,
        // static_cast<HardwareSerial9b>(ser)._ucsrb,
        // static_cast<HardwareSerial9b>(ser)._ucsrc,
        // static_cast<HardwareSerial9b>(ser)._udr
    )
{ }

void HardwareSerial9b::begin(unsigned long baud)
{
    HardwareSerial::begin(baud);
    sbi(*_ucsrb, UCSZ02); // set 9-bit data mode
}

void HardwareSerial9b::end()
{
    HardwareSerial::end();
    cbi(*_ucsrc, UCSZ02);
}

int16_t HardwareSerial9b::read9b()
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        unsigned int c = _rx_buffer[_rx_buffer_tail];
        bool bit9      = _rx_buffer_bit9[_rx_buffer_tail];
        _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
        return c | (bit9 << 8);
    }
}

void HardwareSerial9b::write9b(uint16_t b)
{
    if (_tx_buffer_head == _tx_buffer_tail && bit_is_set(*_ucsra, UDRE0)) {
        if (b & 0x100) {
            *_ucsrb |= bit(TXB80); // set
        } else {
            *_ucsrb &= ~bit(TXB80); // clear
        }
    }
    this->write(b & 0xFF);

    // TODO: The TX buffer won't write a 9th bit if it is used
}

void HardwareSerial9b::_tx_udr_empty_irq_9bit(void)
{
    bool bit9 = _tx_buffer_bit9[_tx_buffer_tail];
    if (bit9)
        *_ucsrb |= bit (TXB80);  // set 9th bit
    else
        *_ucsrb &= ~bit (TXB80); // clear 9th bit

    HardwareSerial::_tx_udr_empty_irq();
}

// Re-implementation of HardwareSerial::_rx_complete_irq() with 9-bit data support.
void HardwareSerial9b::_rx_complete_irq_9bit(void)
{
  if (bit_is_clear(*_ucsra, UPE0)) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    bool bit9 = (*_ucsrb & bit(RXB80)) ? 1 : 0;
    unsigned char c = *_udr;
    rx_buffer_index_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail) {
      _rx_buffer[_rx_buffer_head] = c;
      _rx_buffer_bit9[_rx_buffer_head] = bit9;
      _rx_buffer_head = i;
    }
  } else {
    // Parity error, read byte but discard it
    *_udr;
  };
}

