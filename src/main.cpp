#include <Arduino.h>

#include "lights.h"
#include "states.h"
#include "pins.h"

const uint16_t targetLoopDuration = 10;
const uint16_t TIMED_STATE_DURATION_MS = 10 * 1000;

/**
 * -- TODO
 * power off? 
 * - disconnect battery (possible?) 
 * - disconnect after board: easy, check standby usage
 * - deep sleep chip and LED off
 * - deep sleep chip and LED disconnect via MOSFET
 * 
 * 
 * -- usage
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
  for (uint8_t i_b : Pins::BUTTONS)
  {
    bool state = digitalRead(i_b);
    // pullup
    if (!state)
      States::fsm.trigger(i_b);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  Pins::setup();
  Pins::wipe();

  Lights::startup();
  Lights::hello_world();

  States::setup();
}

void loop()
{
  check_buttons();
  States::fsm.run_machine();

  // keep constant loop duration by aligning to target duration
  Lights::sleep(targetLoopDuration - (millis() % targetLoopDuration));
}
