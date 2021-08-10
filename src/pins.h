#pragma once

#include <Arduino.h>

/**
 * https://components101.com/sites/default/files/component_pin/Arduino-Nano-Pinout.png
 *
 *
 *
 * */

namespace Pins {
// LOW is pressed, using buildin pullups
const uint8_t BT_LEFT = 2; // able to interrupt
const uint8_t BT_OFF = 3;  // able to interrupt
const uint8_t BT_RIGHT = 4;
const uint8_t BUTTONS[] = {BT_LEFT, BT_OFF, BT_RIGHT};

// pad to next register to be able to use register interrupts for buttons

// HIGH is on
const uint8_t PWR_LED = 7; // blink with low bat charge

// HIGH is on
const uint8_t BT_LEFT_RING = 8;
const uint8_t BT_OFF_RING = 9;
const uint8_t BT_RIGHT_RING = 10;
const uint8_t LIGHT_RINGS[] = {BT_LEFT_RING, BT_OFF_RING, BT_RIGHT_RING};

// SPI (11..13) is reserved for generic extensions

// hardware SPI ports
const uint8_t DATA_LEDS_FRONT = A0;
const uint8_t CLOCK_LEDS_FRONT = A1;

const uint8_t DATA_LEDS_BACK = A2;
const uint8_t CLOCK_LEDS_BACK = A3;

const uint8_t LED_PINS[] = {DATA_LEDS_FRONT, CLOCK_LEDS_FRONT, DATA_LEDS_BACK, CLOCK_LEDS_BACK};

// I2C (A4, A5) is reserved for gyro extensions

const uint8_t BAT_V = A6;

void setup() {
    for (uint8_t i_b : BUTTONS) {
        pinMode(i_b, INPUT_PULLUP);
    }

    // managed by FastLED
    for (uint8_t i_i : LED_PINS) {
        pinMode(i_i, OUTPUT);
    }

    pinMode(PWR_LED, OUTPUT);

    for (uint8_t i_i : LIGHT_RINGS) {
        pinMode(i_i, OUTPUT);
    }

    // analog does not need a pinMode
}

void clear_output() {
    // actually managed by FastLED, but let's try it
    for (uint8_t i_i : LED_PINS) {
        digitalWrite(i_i, false);
    }

    digitalWrite(PWR_LED, false);

    for (uint8_t i_i : LIGHT_RINGS) {
        digitalWrite(i_i, false);
    }
}

void test_output() {
    digitalWrite(PWR_LED, false);

    digitalWrite(BT_LEFT_RING, false);
    digitalWrite(BT_OFF_RING, true);
    digitalWrite(BT_RIGHT_RING, false);
    delay(200);
    digitalWrite(BT_LEFT_RING, true);
    digitalWrite(BT_OFF_RING, false);
    digitalWrite(BT_RIGHT_RING, true);
    delay(200);
    digitalWrite(BT_LEFT_RING, false);
    digitalWrite(BT_OFF_RING, false);
    digitalWrite(BT_RIGHT_RING, false);

    digitalWrite(PWR_LED, true);
}

} // namespace Pins
