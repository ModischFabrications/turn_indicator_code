#pragma once

#include <Arduino.h>
#define FASTLED_INTERNAL // disable pragma message
#include <FastLED.h>

#include "pins.h"

/**
 * simulate using https://wokwi.com/arduino/libraries/FastLED
 * 
 * https://www.reddit.com/r/FastLED/comments/gpsc89/custom_animations/
 * https://github.com/atuline/FastLED-Demos
 * https://www.reddit.com/r/FastLED/wiki/index/user_examples
 * https://learn.adafruit.com/dotstar-belly-dance-fans/the-code
 * 
 * */

namespace Lights
{
  const uint8_t BRIGHTNESS = 127;
  const uint32_t CURRENT_LIMIT__MA = 500;

  // using separate strips to make wiring and logic easier
  //https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
  const uint8_t N_LEDS_PER_SIDE = 6; // use dense stripes for fluid animations

  const CRGB INDICATOR_COLOR = CRGB::Orange;
  const float INDICATOR_S_PER_CYCLE = 1;
  float indicator_pos = 0;
  uint32_t last_frame__ms = millis();

  // shared indicator logic is okay
  CRGB leds_left[N_LEDS_PER_SIDE];
  CRGB leds_right[N_LEDS_PER_SIDE];

  // TODO: sync to blinker
  const uint16_t TURNING_TOGGLE_DELAY_MS = 500;
  uint32_t last_toggle = 0;

  void setup()
  {
    // max 24MHz, lower is more stable (which is good as we are carelessly routing in parallel)
    FastLED.addLeds<SK9822, Pins::DATA_LEDS_LEFT, Pins::CLOCK_LEDS_LEFT, BGR, DATA_RATE_MHZ(12)>(leds_left, N_LEDS_PER_SIDE);
    FastLED.addLeds<SK9822, Pins::DATA_LEDS_RIGHT, Pins::CLOCK_LEDS_RIGHT, BGR, DATA_RATE_MHZ(12)>(leds_right, N_LEDS_PER_SIDE);

    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT__MA);
    FastLED.setBrightness(BRIGHTNESS);

    FastLED.clear(true);
  }

  void hello_world()
  {
    for (uint8_t i = 0; i < N_LEDS_PER_SIDE; i++)
    {
      leds_left[i] = CRGB::White;
      leds_right[i] = CRGB::White;
      FastLED.show();
      FastLED.delay(100);
      leds_left[i] = CRGB::Black;
      leds_right[i] = CRGB::Black;
    }
  }

  void sleep(uint32_t time_ms)
  {
    FastLED.delay(time_ms);
  }

  void clear()
  {
    FastLED.clear(true);
    for (uint8_t i_i : Pins::INDICATORS)
    {
      digitalWrite(i_i, false);
    }
  }

  void togglePin(uint8_t pin)
  {
    uint32_t now = millis();
    if (now - last_toggle > TURNING_TOGGLE_DELAY_MS)
    {
      last_toggle = now;
      digitalWrite(pin, !digitalRead(pin));
    }
  }

  void reset_indicators()
  {
    indicator_pos = 0;
    last_frame__ms = millis();
  }

  void wipe_leds(CRGB *leds)
  {
    uint32_t now = millis();
    float frametime_s = (now - last_frame__ms * 1.0) / 1000;
    last_frame__ms = now;

    fadeToBlackBy(leds, N_LEDS_PER_SIDE, (frametime_s/INDICATOR_S_PER_CYCLE)*255);   // half length
    fadeToBlackBy(leds, N_LEDS_PER_SIDE, 20);

    indicator_pos += ((frametime_s * (float)N_LEDS_PER_SIDE) / INDICATOR_S_PER_CYCLE);
    while (indicator_pos >= N_LEDS_PER_SIDE - 1)
      indicator_pos -= (N_LEDS_PER_SIDE);

    uint8_t upper_neighbor = (uint8_t)ceil(indicator_pos);
    uint8_t upper_scale = (upper_neighbor - indicator_pos) * 255;
    leds[upper_neighbor] = INDICATOR_COLOR;
    leds[upper_neighbor].fadeToBlackBy(upper_scale);
    FastLED.show();
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
    reset_indicators();
  }

  void anim_left()
  {
    togglePin(Pins::BT_LEFT_IND);

    wipe_leds(leds_left);
  }

  void turn_right()
  {
    Serial.println(F("turning right"));
    clear();
    reset_indicators();
  }

  void anim_right()
  {
    togglePin(Pins::BT_RIGHT_IND);

    wipe_leds(leds_right);
  }

} // namespace Lights