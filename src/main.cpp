#include <Arduino.h>

#include "lights.h"
#include "states.h"
#include "buttons.h"

const uint16_t targetLoopDuration = 10;
const uint16_t TIMED_STATE_DURATION_MS = 10 * 1000;

/**
 * [LEFT] [OFF] [RIGHT]
 * (short press direction for 10s timer -> indicator blinks)
 * long press for continuous -> indicator on
 * 
 * short press OFF to turn off
 * (long press OFF to turn on stand lights (always on red dot) -> indicator on)
 * 
 * idempotent inputs instead of toggles, resists bouncing and is easier to understand
 * 
 * -- philosophy
 * "A Class with one instance is like a group with one person. Use a namespace instead"
 * classes won't allow pointers and make anonymous function usage difficult
 * */

void check_buttons()
{
  for (uint8_t i_b : BUTTONS)
  {
    bool state = digitalRead(i_b);
    // pullup
    if (!state)
      StateManager::fsm.trigger(i_b);
  }
}

void setup()
{
  for (uint8_t i_b : BUTTONS)
  {
    pinMode(i_b, INPUT_PULLUP);
  }

  startup();
  hello_world();
}

void loop()
{
  StateManager::fsm.run_machine();

  // keep constant loop duration by aligning to target duration
  sleep(targetLoopDuration - (millis() % targetLoopDuration));
}
