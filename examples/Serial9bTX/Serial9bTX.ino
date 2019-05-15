#include <Serial9b.h>

void setup()
{
    Serial9b1.begin(9600);
}

uint8_t b = 0;

void loop()
{
    Serial9b1.write9b(b | BIT9);
    Serial9b1.write9b(b);
    b++;
    
    delay(100);
}
