#include <Arduino.h>

#include "lights.h"
#include "states.h"
#include "pins.h"

const uint16_t targetLoopDuration = 10;
const uint16_t TIMED_STATE_DURATION_MS = 10 * 1000;

bool power_low = false;

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
  // -> save first_positive_flank_time, compare with

  for (uint8_t i_b : Pins::BUTTONS)
  {
    bool state = digitalRead(i_b);
    // pullup
    if (!state)
      States::fsm.trigger(i_b);
  }
}

void check_power()
{
  uint16_t bat_charge = analogRead(Pins::BAT_CHARGE);
  if (!power_low && bat_charge < 500)
  {
    Serial.print("Power low with a measurement of ");
    Serial.println(bat_charge);
    // TODO: blink PWR_IND
    power_low = true;
  }
  else if (power_low && bat_charge > 800)
  {
    Serial.print("Power restored with a measurement of ");
    Serial.println(bat_charge);
    // TODO: turn on PWR_IND
    power_low = false;
  }
}

void setup()
{
  digitalWrite(Pins::PWR_IND, false);
  Serial.begin(115200);
  Serial.println(F("Starting..."));

  Pins::setup();
  Pins::wipe();

  Lights::startup();
  Lights::hello_world();

  States::setup();

  digitalWrite(Pins::PWR_IND, true);
}

void loop()
{
  check_buttons();
  check_power();
  States::fsm.run_machine(); // timed transitions and on_state calls

  // keep constant loop duration by aligning to target duration
  Lights::sleep(targetLoopDuration - (millis() % targetLoopDuration));
}
