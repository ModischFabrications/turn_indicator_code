#include <Arduino.h>

#include "lights.h"
#include "states.h"
#include "pins.h"
#include "power.h"
#include "sleep.h"

const uint16_t TARGET_LOOP_DURATION = 10;

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
  // init first to deactivate unwanted output currents
  Pins::setup();
  Pins::clear_output();

  Serial.begin(115200);
  Serial.println(F("Starting..."));

  Pins::test_indicators();

  Lights::setup();
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
