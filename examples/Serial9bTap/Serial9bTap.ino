// Example that can intercept traffic between two 9-bit serial devices
//
// Serial0: PC connection (log output)
// Serial1: Device 1
// Serial2: Device 2
//

#include <Serial9b.h>

void setup() {
    Serial.begin(115200);

    Serial9b1.begin(9600);
    Serial9b2.begin(9600);
}

void loop() {
    char msg[16];

    // Serial1 -> Serial2 (& log to Serial0)
    if (Serial9b1.available()) {
        uint16_t b = Serial9b1.read9b();

        snprintf(msg, sizeof(msg), "A: %.2x %.2x",
            (b >> 8),
            (b & 0xFF));
        Serial.println(msg);

        Serial9b2.write9b(b);
    }

    // Serial2 -> Serial1 (& log to Serial0)
    if (Serial9b2.available()) {
        uint16_t b = Serial9b2.read9b();

        snprintf(msg, sizeof(msg), "B: %.2x %.2x",
            (b >> 8),
            (b & 0xFF));
        Serial.println(msg);

        Serial9b1.write9b(b);
    }
}