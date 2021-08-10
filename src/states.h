#pragma once

#include <Arduino.h>

#include "lights.h"
#include "pins.h"

/*
This was using FSM before, but I needed buttons to always trigger the same state, regardless of
previous. This makes a fully-defined FSM stupid and bloated, so this is basically mapping action to
state.
*/
namespace States {
namespace {

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

void enableCountdown() {
    Serial.println(millis());
    off_at = millis() + TIMED_STATE_DURATION_MS;
    Serial.println(off_at);
}

cState convert(uint8_t pin) {
    if (pin == Pins::BT_OFF)
        return cState::OFF;
    if (pin == Pins::BT_OFF * 10)
        return cState::STANDING;
    if (pin == Pins::BT_LEFT)
        return cState::TIMED_LEFT;
    if (pin == Pins::BT_LEFT * 10)
        return cState::LEFT;
    if (pin == Pins::BT_RIGHT)
        return cState::TIMED_RIGHT;
    if (pin == Pins::BT_RIGHT * 10)
        return cState::RIGHT;
    return cState::ERROR;
}

void switchTo(cState state) {
    switch (state) {
    case cState::OFF:
        Lights::turn_off();
        break;
    case cState::STANDING:
        Lights::draw_standlights();
        break;
    case cState::TIMED_LEFT:
        enableCountdown();
    case cState::LEFT:
        Lights::turn_left();
        break;
    case cState::TIMED_RIGHT:
        enableCountdown();
    case cState::RIGHT:
        Lights::turn_right();
        break;
    default:
        break;
    }
    active = state;
    Serial.print(F("Now in state "));
    Serial.println(active);
}

} // namespace

void switchFor(uint8_t pin) {
    Serial.print(F("Switch for "));
    Serial.println(pin);
    switchTo(convert(pin));
}

void tick() {
    if ((active == cState::TIMED_LEFT || active == cState::TIMED_RIGHT) && (millis() > off_at)) {
        Serial.println(F("Timeout, back to off"));
        Serial.println(millis());
        off_at = -1;
        switchTo(cState::OFF);
        return;
    }

    switch (active) {
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
