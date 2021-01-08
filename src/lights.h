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
  const uint8_t BRIGHTNESS = 100;
  const uint32_t CURRENT_LIMIT__MA = 500;

  // TODO: use multiple strips?
  //https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples
  const uint8_t N_LEDS_PER_SIDE = 6; // use dense stripes for fluid animations
  // (sacrificial indicator and voltage stabilizer led on board possible)
  const uint8_t N_ALL_LEDS = N_LEDS_PER_SIDE * 2;

  const CRGB INDICATOR_COLOR = CRGB::Orange;
  const float INDICATOR_S_PER_CYCLE = 0.5;
  float indicator_pos = 0;
  uint32_t last_frame__ms = millis();

  CRGB left_leds[N_LEDS_PER_SIDE];

  const uint16_t TURNING_TOGGLE_DELAY_MS = 500;
  uint32_t last_toggle = 0;

  void setup()
  {
    // max 24MHz, lower is more stable
    FastLED.addLeds<SK9822, Pins::DATA_LEDS, Pins::CLOCK_LEDS, BGR, DATA_RATE_MHZ(12)>(left_leds, N_LEDS_PER_SIDE);
    //FastLED.addLeds<SK9822>(leds, N_LEDS);

    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT__MA);
    FastLED.setBrightness(BRIGHTNESS);

    FastLED.clear(true);
  }

  void hello_world()
  {
    for (uint8_t i = 0; i < N_LEDS_PER_SIDE; i++)
    {
      left_leds[i] = CRGB::White;
      FastLED.show();
      FastLED.delay(100);
      left_leds[i] = CRGB::Black;
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

  void turn_off()
  {
    Serial.println(F("turning off"));
    clear();
  }

  void turn_left()
  {
    Serial.println(F("turning left"));
    clear();
  }

  void anim_left()
  {
    togglePin(Pins::BT_LEFT_IND);

    uint32_t now = millis();
    float frametime_s = (now - last_frame__ms * 1.0) / 1000;
    last_frame__ms = now;

    indicator_pos += ((frametime_s * (float)N_LEDS_PER_SIDE) / INDICATOR_S_PER_CYCLE);
    while (indicator_pos >= N_LEDS_PER_SIDE)
      indicator_pos -= N_LEDS_PER_SIDE;

    uint8_t upper_neighbor = (uint8_t)ceil(indicator_pos);
    uint8_t upper_scale = (upper_neighbor - indicator_pos) * 255;
    uint8_t lower_neighbor = (uint8_t)floor(indicator_pos);
    uint8_t lower_scale = (indicator_pos - lower_neighbor) * 255;

    // wrap around one later to keep soft trail at upper border
    if (upper_neighbor >= N_LEDS_PER_SIDE)
      upper_neighbor = 0;

    FastLED.clear(false);
    //fadeToBlackBy(left_leds, N_LEDS_PER_SIDE, (frametime_s/INDICATOR_S_PER_CYCLE)*3*255);   // half length
    left_leds[upper_neighbor] = INDICATOR_COLOR;
    left_leds[upper_neighbor].fadeToBlackBy(upper_scale);
    left_leds[lower_neighbor] = INDICATOR_COLOR;
    left_leds[lower_neighbor].fadeToBlackBy(lower_scale);
    FastLED.show();
  }

  void turn_right()
  {
    Serial.println(F("turning right"));
    clear();
  }

  void anim_right()
  {
    togglePin(Pins::BT_RIGHT_IND);

    // TODO: wipe
  }

} // namespace Lights