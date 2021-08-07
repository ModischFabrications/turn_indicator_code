#pragma once

#include <Arduino.h>
#include <OneButton.h>

#include "pins.h"
#include "states.h"

namespace Buttons
{
    // I spend 30min searching for a generic way, don't do the same
    // len(Pin::BUTTONS)
    OneButton O_BUTTONS[3];
    OneButton OBT_LEFT;
    OneButton OBT_OFF;
    OneButton OBT_RIGHT;

    // void* roundtrip is necessary as oneButton can't handle other types
    void trigger(void* i)
    {
        int id = (int)i;
        Serial.print("Triggering ");
        Serial.println(id);
        States::fsm.trigger(id);
    }

    void tick()
    {
        for (OneButton o_b : O_BUTTONS)
        {
            o_b.tick();
        }
    }

    void setup()
    {
        for (int i=0; i<3; i++)
        {
            OneButton o_b = OneButton(Pins::BUTTONS[i]);
           o_b.attachClick(trigger,(void*)  i);
           o_b.attachLongPressStart(trigger, (void*) (i*10));
           O_BUTTONS[i] = o_b;
        }

        OBT_LEFT = O_BUTTONS[0];
        OBT_OFF = O_BUTTONS[1];
        OBT_RIGHT = O_BUTTONS[2];
    }

}
