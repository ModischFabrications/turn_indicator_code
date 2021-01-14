#pragma once

#include <Arduino.h>

/**
 * https://components101.com/sites/default/files/component_pin/Arduino-Nano-Pinout.png
 * 
 * 
 * 
 * */

namespace Pins
{
    const uint8_t BT_LEFT = 2; // able to interrupt
    const uint8_t BT_OFF = 3;  // able to interrupt
    const uint8_t BT_RIGHT = 4;
    // LOW is active
    const uint8_t BUTTONS[] = {BT_LEFT, BT_OFF, BT_RIGHT};

    // hardware SPI ports
    const uint8_t DATA_LEDS = 11;
    const uint8_t CLOCK_LEDS = 13;

    // HIGH is active
    const uint8_t PWR_IND = 7; // blink with low bat charge

    const uint8_t BT_LEFT_IND = 8;
    const uint8_t BT_OFF_IND = 9;
    const uint8_t BT_RIGHT_IND = 10;
    // HIGH is active
    const uint8_t INDICATORS[] = {BT_LEFT_IND, BT_OFF_IND, BT_RIGHT_IND};

    const uint8_t BAT_V = A6;

    void setup()
    {
        for (uint8_t i_b : BUTTONS)
        {
            pinMode(i_b, INPUT_PULLUP);
        }

        // managed by FastLED
        pinMode(DATA_LEDS, OUTPUT);
        pinMode(CLOCK_LEDS, OUTPUT);

        pinMode(PWR_IND, OUTPUT);

        for (uint8_t i_i : INDICATORS)
        {
            pinMode(i_i, OUTPUT);
        }

        // analog does not need a pinMode
    }

    void clear_output()
    {
        // actually managed by FastLED, but let's try it
        digitalWrite(DATA_LEDS, false);
        digitalWrite(CLOCK_LEDS, false);

        digitalWrite(PWR_IND, false);

        for (uint8_t i_i : INDICATORS)
        {
            digitalWrite(i_i, false);
        }
    }

    void test_indicators()
    {
        digitalWrite(PWR_IND, false);

        digitalWrite(BT_LEFT_IND, false);
        digitalWrite(BT_OFF_IND, true);
        digitalWrite(BT_RIGHT_IND, false);
        delay(200);
        digitalWrite(BT_LEFT_IND, true);
        digitalWrite(BT_OFF_IND, false);
        digitalWrite(BT_RIGHT_IND, true);
        delay(200);
        digitalWrite(BT_LEFT_IND, false);
        digitalWrite(BT_OFF_IND, false);
        digitalWrite(BT_RIGHT_IND, false);

        digitalWrite(PWR_IND, true);
    }

} // namespace Pins
