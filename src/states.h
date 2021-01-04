#pragma once

#include <Arduino.h>
#include <Fsm.h>

#include "lights.h"
#include "buttons.h"

// pin numbers are unique by design
// *10 for long press
const uint8_t EV_OFF = PIN_BT_OFF;
const uint8_t EV_LEFT = PIN_BT_LEFT;
const uint8_t EV_RIGHT = PIN_BT_RIGHT;

namespace StateManager
{
    // https://github.com/jonblack/arduino-fsm/blob/master/examples/timed_switchoff/timed_switchoff.ino
    State off_state(turn_off, NULL, NULL);
    State left_state(turn_left, anim_left, NULL);
    State right_state(turn_right, anim_right, NULL);

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
} // namespace StateManager
