#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>

// Char table definitions
const unsigned char LED_0F[] = { 
  // 0   1    2    3  4  5    6    7  8  9    A    b  C    d    E    F    -
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xBF, 0x7F, 0xFF
};

class LEDController {
public:
    LEDController(uint8_t dioPin, uint8_t sclkPin, uint8_t rclkPin);
    void begin();

    void display();
    void display(int num1, int num2, int num3, int num4);
    void fillArray(int num1, int num2, int num3, int num4);

private:
    unsigned char LED[16];
    uint8_t DIO, SCLK, RCLK;

    void output(unsigned char X);
};

#endif
