#pragma once

#include <Arduino.h>

#include "lights.h"
#include "pins.h"

namespace States
{
    const uint16_t TIMED_STATE_DURATION_MS = 5 * 1000;

    enum cState {
        ERROR = 0,
        OFF,
        STANDING,
        TIMED_LEFT,
        LEFT,
        TIMED_RIGHT,
        RIGHT,
    };

    cState active = cState::OFF;
    uint32_t off_at = -1;

    void scheduleOff()
    {
        Serial.println(millis());
        off_at = millis() + TIMED_STATE_DURATION_MS;
        Serial.println(off_at);
    }

    cState convert(uint8_t pin)
    {
        if (pin == Pins::BT_OFF) return cState::OFF;
        if (pin == Pins::BT_OFF*10) return cState::STANDING;
        if (pin == Pins::BT_LEFT) return cState::TIMED_LEFT;
        if (pin == Pins::BT_LEFT*10) return cState::LEFT;
        if (pin == Pins::BT_RIGHT) return cState::TIMED_RIGHT;
        if (pin == Pins::BT_RIGHT*10) return cState::RIGHT;
        return cState::ERROR;
    }

    void switchTo(cState state)
    {
        switch (state)
        {
        case cState::OFF:
            Lights::turn_off();
            break;
        case cState::STANDING:
            Lights::draw_standlights();
            break;
        case cState::TIMED_LEFT:
            scheduleOff();
        case cState::LEFT:
            Lights::turn_left();
            break;
        case cState::TIMED_RIGHT:
            scheduleOff();
        case cState::RIGHT:
            Lights::turn_right();
            break;
        default:
            break;
        }
        active = state;
        Serial.print("Now in state ");
        Serial.println(active);
    }

    void switchFor(uint8_t pin)
    {
        Serial.print("Switch for ");
        Serial.println(pin);
        switchTo(convert(pin));
    }

    void tick()
    {
        if ((active == cState::TIMED_LEFT || active == cState::TIMED_RIGHT) && (millis() > off_at))
        {
            Serial.println("Timeout, back to off");
            Serial.println(millis());
            off_at = -1;
            switchTo(cState::OFF);
            return;
        }

        switch (active)
        {
        case cState::TIMED_LEFT:
        case cState::LEFT:
            Lights::anim_left();
            break;
        case cState::TIMED_RIGHT:
        case cState::RIGHT:
            Lights::anim_right();
            break;
        default:
            break;
        }
    }


} // namespace States
