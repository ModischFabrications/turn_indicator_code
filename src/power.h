#pragma once

#include <Arduino.h>

#include "pins.h"

namespace Power
{
    const uint16_t LOW_PWR_THRESHOLD = 790;  // ~3.8V
    const uint16_t HIGH_PWR_THRESHOLD = 850; // ~4.1V

    bool power_low = false;

    const uint16_t PWR_TOGGLE_DELAY_MS = 500;
    uint32_t last_toggle = 0;

    void setup()
    {
        Serial.print(F("Initial power reading is "));
        Serial.println(analogRead(Pins::BAT_CHARGE));
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

    void check_power()
    {
        uint16_t bat_charge = analogRead(Pins::BAT_CHARGE);
        if (!power_low && bat_charge < 500)
        {
            Serial.print("Power low with a measurement of ");
            Serial.println(bat_charge);
            power_low = true;
        }
        else if (power_low && bat_charge > 800)
        {
            Serial.print("Power restored with a measurement of ");
            Serial.println(bat_charge);
            power_low = false;
            digitalWrite(Pins::PWR_IND, true);
        }

        if (power_low)
            blink();
    }

    // hibernate? turn off LEDs with MOSFET?

} // namespace Power
