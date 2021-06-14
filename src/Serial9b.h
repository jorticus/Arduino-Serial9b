/*
  Serial9b.h - HardwareSerial extension that supports 9-bit data
  Copyright (c) 2019 Jared Sanson

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SERIAL9B_H
#define SERIAL9B_H


#include <Arduino.h>

#if !defined (__AVR__) && !defined (__XTENSA__)
#error Unsupported Architecture
#endif

#define SERIAL_9N1 0x40

#ifndef BIT8
#define BIT8 (0x100)
#endif


class Stream9b : public Stream
{
public:
    virtual int16_t read9b() = 0;
    virtual void write9b(uint16_t b) = 0;
};

template <typename T>
class SoftwareSerial9bWrapper : public Stream9b
{
public:
    SoftwareSerial9bWrapper(T& ser)
        : ser(ser)
    { }

    int available() override { return ser.available(); }
    int read() override { return (uint8_t)ser.read(); }
    int peek() override { return ser.peek(); }
    void flush() override { ser.flush(); }

    size_t write(uint8_t b) override { return ser.write((uint16_t)b); }

    int16_t read9b() override { return ser.read(); }
    void write9b(uint16_t b) override { ser.write(b); }

private:
    T& ser;
};



#if defined (__AVR__)
// https://arduino.stackexchange.com/questions/24744/is-it-possible-to-receive-a-9-bit-protocol-on-the-arduino-uno-with-arduino
// https://forum.arduino.cc/index.php?topic=91377.0
// https://citizengadget.com/post/22556427458/altering-new-soft-serial-nss-for-9-bit-cans-of
// https://github.com/arduino/Arduino/pull/2291/files
// https://github.com/arduino/Arduino/pull/2292/files
// https://forum.arduino.cc/index.php?topic=517063.0
// https://github.com/sirleech/NewSoftSerial
// http://gammon.com.au/Arduino/hardwareSerial_diffs.txt
// https://github.com/addibble/SoftwareSerial9

#include <HardwareSerial.h>

class HardwareSerial9b : public HardwareSerial, public Stream9b
{
public:
    HardwareSerial9b(
        volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
        volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
        volatile uint8_t *ucsrc, volatile uint8_t *udr);
        // : HardwareSerial(ubrrh, ubrrl, ucsra, ucsrb, ucsrc, udr)
    //{ }

    HardwareSerial9b(const HardwareSerial& ser);
    //     : HardwareSerial(
    //         0,0,0,0,0,0
    //         // TODO: This seems to crash the MCU?
    //         // static_cast<HardwareSerial9b>(ser)._ubrrh,
    //         // static_cast<HardwareSerial9b>(ser)._ubrrl,
    //         // static_cast<HardwareSerial9b>(ser)._ucsra,
    //         // static_cast<HardwareSerial9b>(ser)._ucsrb,
    //         // static_cast<HardwareSerial9b>(ser)._ucsrc,
    //         // static_cast<HardwareSerial9b>(ser)._udr
    //     )
    // { }

    void _rx_complete_irq_9bit(void);
    void _tx_udr_empty_irq_9bit(void);

    void begin(unsigned long baud);
    void end();

    int16_t read9b() override;
    void write9b(uint16_t b) override;

protected:
    // TODO: Pack this efficiently. It's a little heavy on memory...
    unsigned char _rx_buffer_bit9[SERIAL_RX_BUFFER_SIZE];
    unsigned char _tx_buffer_bit9[SERIAL_RX_BUFFER_SIZE];
};

#if defined(UBRRH) || defined(UBRRH0H)
// Don't enable 9-bit serial for UART0, since it is used for programming/serial console!
#endif
#if defined(UBRR1H)
extern HardwareSerial9b Serial9b1;
#endif
#if defined(UBRR2H)
extern HardwareSerial9b Serial9b2;
#endif
#if defined(UBRR3H)
extern HardwareSerial9b Serial9b3;
#endif
#endif


#endif /* SERIAL9B_H */