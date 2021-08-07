#pragma once

#include <Arduino.h>
#include <Fsm.h>

#include "lights.h"
#include "pins.h"

namespace States
{
    const uint16_t TIMED_STATE_DURATION_MS = 5 * 1000;

    // pin numbers are unique by design
    // *10 for long press
    const uint8_t EV_OFF = Pins::BT_OFF;
    const uint8_t EV_LEFT = Pins::BT_LEFT;
    const uint8_t EV_RIGHT = Pins::BT_RIGHT;

    // https://github.com/jonblack/arduino-fsm/blob/master/examples/timed_switchoff/timed_switchoff.ino
    State off_state(Lights::turn_off, NULL, NULL);
    State stand_state(Lights::draw_standlights, NULL, NULL);

    State timed_left_state(Lights::turn_left, Lights::anim_left, NULL);         // identical behaviour, split just for FSM identification
    State left_state(Lights::turn_left, Lights::anim_left, NULL);
    State timed_right_state(Lights::turn_right, Lights::anim_right, NULL);
    State right_state(Lights::turn_right, Lights::anim_right, NULL);

    Fsm fsm(&off_state);

    void register_transitions()
    {
        //fsm.add_transition(&left_state, &off_state, EV_OFF, NULL);
        fsm.add_timed_transition(&timed_left_state, &off_state, TIMED_STATE_DURATION_MS, NULL);
        fsm.add_timed_transition(&timed_right_state, &off_state, TIMED_STATE_DURATION_MS, NULL);
        
        fsm.add_transition(&left_state, &off_state, EV_OFF, NULL);
        fsm.add_transition(&right_state, &off_state, EV_OFF, NULL);
        fsm.add_transition(&left_state, &stand_state, EV_OFF*10, NULL);
        fsm.add_transition(&right_state, &stand_state, EV_OFF*10, NULL);

        fsm.add_transition(&off_state, &timed_left_state, EV_LEFT, NULL);
        fsm.add_transition(&right_state, &timed_left_state, EV_LEFT, NULL);
        fsm.add_transition(&off_state, &left_state, EV_LEFT*10, NULL);
        fsm.add_transition(&right_state, &left_state, EV_LEFT*10, NULL);

        fsm.add_transition(&off_state, &timed_right_state, EV_RIGHT, NULL);
        fsm.add_transition(&left_state, &timed_right_state, EV_RIGHT, NULL);
        fsm.add_transition(&off_state, &right_state, EV_RIGHT*10, NULL);
        fsm.add_transition(&left_state, &right_state, EV_RIGHT*10, NULL);
    }
} // namespace States
