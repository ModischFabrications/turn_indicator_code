#pragma once

#include <Arduino.h>

#include "pins.h"

namespace Power
{
    const float BAT_EMPTY = 3.8;
    const float BAT_FULL = 4.0;

    bool power_low = false;

    const uint16_t PWR_TOGGLE_DELAY_MS = 500;
    uint32_t last_toggle = 0;

    void setup()
    {
        Serial.print(F("Initial power reading is "));
        Serial.println(analogRead(Pins::BAT_V));
        digitalWrite(Pins::PWR_IND, true);
    }

    void blink()
    {
        uint32_t now = millis();
        if (now - last_toggle > PWR_TOGGLE_DELAY_MS)
        {
            last_toggle = now;
            digitalWrite(Pins::PWR_IND, !digitalRead(Pins::PWR_IND));
        }
    }

    float toVolt(uint16_t reading)
    {
        return (5.0/1024)*reading;
    }

    void check_power()
    {
        float bat_voltage = toVolt(analogRead(Pins::BAT_V));
        //Serial.println(bat_voltage);
        if (!power_low && bat_voltage < BAT_EMPTY)
        {
            Serial.print("Power low with a measurement of ");
            Serial.println(bat_voltage);
            power_low = true;
        }
        else if (power_low && bat_voltage > BAT_FULL)
        {
            Serial.print("Power restored with a measurement of ");
            Serial.println(bat_voltage);
            power_low = false;
            digitalWrite(Pins::PWR_IND, true);
        }

        if (power_low)
            blink();
    }

    // hibernate? turn off LEDs with MOSFET?

} // namespace Power
