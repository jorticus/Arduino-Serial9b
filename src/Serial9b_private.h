#ifndef SERIAL9B_PRIVATE_H
#define SERIAL9B_PRIVATE_H

#include "Serial9b.h"
#include <HardwareSerial_private.h>
#include "wiring_private.h"

#if !(defined(HAVE_HWSERIAL0) || defined(HAVE_HWSERIAL1) || defined(HAVE_HWSERIAL2) || defined(HAVE_HWSERIAL3))
#   error HWSerial not available
#endif

#if !defined(UCSZ02)
#   if defined(UCSZ2)
#       define UCSZ02 UCSZ2
#       define TXB80  TXB8
#       define RXB80  RXB8
#   elif defined(UCSZ12)
#       define UCSZ02 UCSZ12
#       define TXB80  TXB81
#       define RXB80  RXB81
#   else
#       error No 9-bit capable UART found in Serial9b.h
#   endif
#endif

#endif /* SERIAL9B_PRIVATE_H */