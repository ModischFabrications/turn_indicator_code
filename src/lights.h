#pragma once

#include <Arduino.h>
#define FASTLED_INTERNAL // disable pragma message
#include <FastLED.h>

#include "pins.h"

namespace Lights
{

  const uint32_t CURRENT_LIMIT__MA = 1000;

  const uint8_t N_LED_PER_SIDE = 10;
  // + sacrificial indicator and voltage stabilizer led on board
  const uint8_t N_LEDS = N_LED_PER_SIDE * 2 + 1;

  const CRGB INDICATOR_COLOR = CRGB::LightGoldenrodYellow;

  CRGB leds[N_LEDS];

  uint32_t last_toggle = 0;
  const uint16_t toggle_delay = 100;

  void startup()
  {
    FastLED.addLeds<SK9822, Pins::DATA_LEDS, Pins::CLOCK_LEDS>(leds, N_LEDS);

    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT__MA);
    FastLED.setBrightness(200);

    FastLED.showColor(CRGB::Black);
  }

  void hello_world()
  {
  }

  void sleep(uint32_t time_ms)
  {
    FastLED.delay(time_ms);
  }

  void clear()
  {
    FastLED.showColor(CRGB::Black);
    for (uint8_t i_i : Pins::INDICATORS)
    {
      digitalWrite(i_i, false);
    }
  }

  void togglePin(uint8_t pin)
  {
    uint32_t now = millis();
    if (now - last_toggle > toggle_delay)
    {
      last_toggle = now;
      digitalWrite(pin, !digitalRead(pin));
    }
  }

  void turn_off()
  {
    Serial.println(F("turning off"));
    clear();
  }

  void turn_left()
  {
    Serial.println(F("turning left"));
    clear();
    // digitalWrite(Pins::BT_LEFT_IND, true);
  }

  void anim_left()
  {
    togglePin(Pins::BT_LEFT_IND);

    // TODO: wipe
  }

  void turn_right()
  {
    Serial.println(F("turning right"));
    clear();
    // digitalWrite(Pins::BT_RIGHT_IND, true);
  }

  void anim_right()
  {
    togglePin(Pins::BT_RIGHT_IND);

    // TODO: wipe
  }

} // namespace Lights