#pragma once

#include <Arduino.h>
#include <Fsm.h>

#include "lights.h"
#include "pins.h"

namespace States
{
    const uint16_t TIMED_STATE_DURATION_MS = 10 * 1000;

    // pin numbers are unique by design
    // *10 for long press
    const uint8_t EV_OFF = Pins::BT_OFF;
    const uint8_t EV_LEFT = Pins::BT_LEFT;
    const uint8_t EV_RIGHT = Pins::BT_RIGHT;

    // https://github.com/jonblack/arduino-fsm/blob/master/examples/timed_switchoff/timed_switchoff.ino
    State off_state(Lights::turn_off, NULL, NULL);
    State left_state(Lights::turn_left, Lights::anim_left, NULL);
    State right_state(Lights::turn_right, Lights::anim_right, NULL);

    Fsm fsm(&off_state);

    void setup()
    {
        //fsm.add_timed_transition(&off_state, &left_state, TIMED_STATE_DURATION_MS, NULL);
        //fsm.add_transition(&left_state, &off_state, EV_OFF, NULL);

        fsm.add_transition(&left_state, &off_state, EV_OFF, NULL);
        fsm.add_transition(&right_state, &off_state, EV_OFF, NULL);

        fsm.add_transition(&off_state, &left_state, EV_LEFT, NULL);
        fsm.add_transition(&right_state, &left_state, EV_LEFT, NULL);

        fsm.add_transition(&off_state, &right_state, EV_RIGHT, NULL);
        fsm.add_transition(&left_state, &right_state, EV_RIGHT, NULL);
    }
} // namespace States
