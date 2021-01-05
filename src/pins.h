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
    const uint8_t BUTTONS[] = {BT_LEFT, BT_OFF, BT_RIGHT};

    const uint8_t DATA_LEDS = 5;
    const uint8_t CLOCK_LEDS = 6;

    const uint8_t PWR_IND = 7; // blink with low bat charge

    const uint8_t BT_LEFT_IND = 8;
    const uint8_t BT_OFF_IND = 9;
    const uint8_t BT_RIGHT_IND = 10;
    const uint8_t INDICATORS[] = {BT_LEFT_IND, BT_OFF_IND, BT_RIGHT_IND};

    const uint8_t BAT_CHARGE = A6;

    void setup()
    {
        for (uint8_t i_b : BUTTONS)
        {
            pinMode(i_b, INPUT_PULLUP);
        }

        pinMode(DATA_LEDS, OUTPUT);
        pinMode(CLOCK_LEDS, OUTPUT);

        pinMode(PWR_IND, OUTPUT);

        pinMode(BT_LEFT_IND, OUTPUT);
        pinMode(BT_OFF_IND, OUTPUT);
        pinMode(BT_RIGHT_IND, OUTPUT);

        // analog does not need a pinMode
    }

    void wipe()
    {
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
    }

} // namespace PINS
