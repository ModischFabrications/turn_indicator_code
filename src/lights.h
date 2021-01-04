#pragma once

#include <Arduino.h>
#define FASTLED_INTERNAL // disable pragma message
#include <FastLED.h>

const uint8_t PIN_LEDS = 1;
const uint8_t PIN_CLOCK_LEDS = 2;

const uint32_t CURRENT_LIMIT__MA = 1000;

const uint8_t N_LED_PER_SIDE = 10;
// + sacrificial indicator and voltage stabilizer led on board
const uint8_t N_LEDS = N_LED_PER_SIDE * 2 + 1;

const CRGB INDICATOR_COLOR = CRGB::LightGoldenrodYellow;

CRGB leds[N_LEDS];

void startup()
{
  pinMode(PIN_LEDS, OUTPUT);
  FastLED.addLeds<SK9822, PIN_LEDS, PIN_CLOCK_LEDS>(leds, N_LEDS);

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

void turn_off()
{
  FastLED.showColor(CRGB::Black);
}

void turn_left()
{
  Serial.println(F("turning left"));
  turn_off();
}

void anim_left()
{
  // TODO: wipe
}

void turn_right()
{
  Serial.println(F("turning right"));
  turn_off();
}

void anim_right()
{
  // TODO: wipe
}
