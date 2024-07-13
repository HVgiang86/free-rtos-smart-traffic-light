#include "LEDController.h"

LEDController::LEDController(uint8_t dioPin, uint8_t sclkPin, uint8_t rclkPin)
    : DIO(dioPin), SCLK(sclkPin), RCLK(rclkPin) {
}

void LEDController::begin()
{
    pinMode(SCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(DIO, OUTPUT);
}

void LEDController::display()
{
    byte digit = 12;
    for (byte dselect = 1; dselect < 0x10; dselect = dselect << 1) {
        output(LED_0F[LED[digit]]);
        output(dselect);
        output(LED_0F[LED[digit - 4]]);
        output(dselect);
        output(LED_0F[LED[digit - 8]]);
        output(dselect);
        output(LED_0F[LED[digit - 12]]);
        output(dselect);
        digitalWrite(RCLK, LOW);
        digitalWrite(RCLK, HIGH);
        ++digit;
    }
}

void LEDController::display(int num1, int num2, int num3, int num4)
{
    fillArray(num1, num2, num3, num4);
    display();
}

void LEDController::fillArray(int num1, int num2, int num3, int num4)
{
    // Extract digits from num1
    LED[0] = num1 % 10;
    num1 /= 10;
    LED[1] = num1 % 10;
    num1 /= 10;
    LED[2] = num1 % 10;
    num1 /= 10;
    // LED[3] = num1 % 10;
    LED[3] = 18;

    // Extract digits from num2
    LED[4] = num2 % 10;
    num2 /= 10;
    LED[5] = num2 % 10;
    num2 /= 10;
    LED[6] = num2 % 10;
    num2 /= 10;
    LED[7] = num2 % 10;
    LED[7] = 18;

    // Extract digits from num3
    LED[8] = num3 % 10;
    num3 /= 10;
    LED[9] = num3 % 10;
    num3 /= 10;
    LED[10] = num3 % 10;
    num3 /= 10;
    // LED[11] = num3 % 10;
    LED[11] = 18;

    // Extract digits from num4
    LED[12] = num4 % 10;
    num4 /= 10;
    LED[13] = num4 % 10;
    num4 /= 10;
    LED[14] = num4 % 10;
    num4 /= 10;
    // LED[15] = num4 % 10;
    LED[15] = 18;
}

void LEDController::output(unsigned char X) {
    for (unsigned char i = 8; i >= 1; i--) {

        // SEND MSB bit
        digitalWrite(DIO, X & 0x80 ? HIGH : LOW);
        X <<= 1;
        digitalWrite(SCLK, LOW);
        digitalWrite(SCLK, HIGH);
    }
}
