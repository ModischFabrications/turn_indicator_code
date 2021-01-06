#include <Arduino.h>

#include "lights.h"
#include "states.h"
#include "pins.h"
#include "power.h"

const uint16_t TARGET_LOOP_DURATION = 10;

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
  // TODO: long press detection
  // -> save first_positive_flank_time, compare with now for hold time

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
  Pins::test_indicators();

  Lights::startup();
  Lights::hello_world();

  States::setup();

  Power::setup();
}

void loop()
{
  check_buttons();
  Power::check_power();

  States::fsm.run_machine(); // timed transitions and on_state calls

  // keep constant loop duration by aligning to target duration
  Lights::sleep(TARGET_LOOP_DURATION - (millis() % TARGET_LOOP_DURATION));
}
