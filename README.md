HardwareSerial wrapper to enable 9-bit TX/RX

Provides `Serial9b1` object that wraps the standard `Serial1` object,
and defines the following additional methods:

    int16_t   read9b();
    void      write9b(uint16_t b);

Usage:

    void setup() {
        // Configure Serial1 for 9-bit data.
        // Do NOT use Serial1 while this is in use!
        Serial9b1.begin(9600);
    }

    void loop() {
        Serial9b1.write9b(0x1AA); // bit9 = 1
        Serial9b1.write9b(0x0AA); // bit9 = 0
        Serial9b1.write9b(0xAA | BIT9); // bit9 = 1 (alternative method)
        delay(10);
    }
