// Loopback test
// Connect TX & RX together to validate that we can both transmit and receive 9-bit data

#include <Serial9b.h>

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.begin(9600);

    Serial9b1.begin(9600);
}

uint8_t i = 0;
bool received = true;

void loop()
{
    uint16_t b = i | (1 << 8);
    Serial.print("TX: "); Serial.println(b, 16);
    digitalWrite(LED_BUILTIN, LOW);
    Serial9b1.write9b(b);
    //Serial1.write(b);

    delay(25);

    if (Serial9b1.available()) {
        int16_t val = Serial9b1.read9b();
        Serial.print("RX: "); Serial.println(val, 16);

        /*int16_t val = Serial9b1.read9b();
        if (val >= 0) {
            Serial.print("RX: "); Serial.println(val, 16);
            bool bit9 = (val & 0x100);
            if (!bit9 || (b != val)) {
                digitalWrite(LED_BUILTIN, HIGH);

                if (!bit9)
                    Serial.println("ERROR: Bit9 not set");
                if (b != val)
                    Serial.println("ERROR: RX != TX");
            }
        }*/
    }

    i++;
    delay(25);
}
