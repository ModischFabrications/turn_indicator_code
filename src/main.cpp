#include <Arduino.h>

#include "lights.h"
#include "states.h"
#include "pins.h"
#include "power.h"
#include "sleep.h"
#include "buttons.h"

const uint16_t TARGET_LOOP_DURATION__MS = 10;

void setup()
{
  // init first to deactivate unwanted output currents
  Pins::setup();
  Pins::clear_output();

  Serial.begin(115200);
  Serial.println(F("Starting..."));

  Buttons::setup();
  Lights::setup();
  Power::setup();

  Pins::test_output();

  Lights::hello_world();

  States::register_transitions();
}

void loop()
{
  Buttons::tick();
  Power::check_power();

  States::fsm.run_machine(); // timed transitions and on_state calls

  // keep constant loop duration by aligning to target duration
  Lights::sleep(TARGET_LOOP_DURATION__MS - (millis() % TARGET_LOOP_DURATION__MS));
}
