#pragma once

#include <Arduino.h>
#include <avr/sleep.h>

#include "pins.h"

namespace Sleep
{

    void INT_from_BT_OFF()
    {
        // nothing to do yet...
    }

    void sleep()
    {
        attachInterrupt(digitalPinToInterrupt(Pins::BT_OFF), INT_from_BT_OFF, FALLING);

        Serial.println(F("Snoozing, waiting for trigger to turn on again..."));
        delay(1000);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
        // waiting for hardware interrupt or power cycle
        sleep_disable();
    }
} // namespace Sleep
