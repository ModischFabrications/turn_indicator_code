#pragma once

#include <Arduino.h>
#define FASTLED_INTERNAL // disable pragma message
#include <FastLED.h>

#include "pins.h"
#include "shared/serialWrapper.h"

/**
 * simulate using https://wokwi.com/arduino/libraries/FastLED
 *
 * References:
 * https://www.reddit.com/r/FastLED/comments/gpsc89/custom_animations/
 * https://github.com/atuline/FastLED-Demos
 * https://www.reddit.com/r/FastLED/wiki/index/user_examples
 * https://learn.adafruit.com/dotstar-belly-dance-fans/the-code
 *
 * This class should honor different timescales, but call into it periodically for proper animations
 * */

namespace Lights {
namespace {
const uint8_t BRIGHTNESS = 127;
const uint32_t CURRENT_LIMIT__MA = 500;

// physical strip has *2 because both sides are chained through
// going from last left to last right
// needs to be even!
const uint8_t N_LEDS_PER_STRIPE = 40; // fluid animations need a high density
const uint8_t N_LEDS_PER_SIDE = N_LEDS_PER_STRIPE / 2;

const uint8_t FRONT_LIGHT_HUE = 38;
// make sure that your values stay inside 0..N_LEDS_PER_SIDE
const float FRONT_LIGHT_MIDDLE = N_LEDS_PER_SIDE / 3.0;
const float FRONT_LIGHT_WIDTH = N_LEDS_PER_SIDE / 4.0;

const uint8_t TAIL_LIGHTS_HUE = 0;
const float TAIL_LIGHT_MIDDLE = N_LEDS_PER_SIDE / 2.0;
const float TAIL_LIGHT_WIDTH = N_LEDS_PER_SIDE / 3.0;

const float SMOOTHING = 1.5;

const CRGB INDICATOR_COLOR = CRGB::Orange;
// shared indicator logic is okay
const float INDICATOR_S_PER_CYCLE = 1.5;
float indicator_pos = 0;
uint32_t last_frame__ms = millis();

CRGB leds_front[N_LEDS_PER_STRIPE];
CRGB leds_back[N_LEDS_PER_STRIPE];

// TODO: sync to blinker
const uint16_t TURNING_TOGGLE_DELAY_MS = 500;
uint32_t last_toggle = 0;

float brightness_in_wipe(float position, float middle, float width) {
    // TODO: add cubic, sin or similar
    return max(0, 1 - (abs(position - middle) / (width / 2.0)));
}

void mirror(CRGB leds[]) {
    for (uint8_t i = 0; i < N_LEDS_PER_STRIPE; i++) {
        leds[N_LEDS_PER_STRIPE - i - 1] = leds[i];
    }
}

void clear() {
    FastLED.clear(true);
    for (uint8_t i_i : Pins::LIGHT_RINGS) {
        digitalWrite(i_i, false);
    }
}

void togglePin(uint8_t pin) {
    uint32_t now = millis();
    if (now - last_toggle > TURNING_TOGGLE_DELAY_MS) {
        last_toggle = now;
        digitalWrite(pin, !digitalRead(pin));
    }
}

void reset_indicators() {
    indicator_pos = 0;
    last_frame__ms = millis();
    last_toggle = millis();
}

void update_indicator_pos() {
    uint32_t now = millis();
    float frametime_s = (now - last_frame__ms * 1.0) / 1000;
    last_frame__ms = now;

    float increment = ((frametime_s * (float)N_LEDS_PER_SIDE) / INDICATOR_S_PER_CYCLE);

    indicator_pos += increment;

    // wrap around
    while (indicator_pos >= N_LEDS_PER_SIDE - 1)
        indicator_pos -= N_LEDS_PER_SIDE;

    fadeToBlackBy(leds_front, N_LEDS_PER_STRIPE, frametime_s * SMOOTHING * 255.0);
    fadeToBlackBy(leds_back, N_LEDS_PER_STRIPE, frametime_s * SMOOTHING * 255.0);
}

void wipe_smooth(CRGB leds[], bool rising) {
    uint8_t upper_neighbor = (uint8_t)ceil(indicator_pos);
    uint8_t lower_neighbor = upper_neighbor - 1;
    if (lower_neighbor > N_LEDS_PER_SIDE)
        lower_neighbor += N_LEDS_PER_SIDE;

    if (!rising) {
        upper_neighbor = N_LEDS_PER_SIDE - 1 - upper_neighbor;
        lower_neighbor = N_LEDS_PER_SIDE - 1 - lower_neighbor;
    }
    leds[lower_neighbor] = INDICATOR_COLOR;
    leds[upper_neighbor] = INDICATOR_COLOR;
    leds[upper_neighbor] %= 255.0 * (indicator_pos - floor(indicator_pos));
}

void wipe(CRGB leds[], bool rising) {
    uint8_t upper_neighbor = (uint8_t)ceil(indicator_pos);
    if (!rising)
        upper_neighbor = N_LEDS_PER_SIDE - 1 - upper_neighbor;
    leds[upper_neighbor] = INDICATOR_COLOR;
}

} // namespace

void turn_off() {
    println(F("turning off"));
    clear();
}

void draw_standlights() {
    clear();

    uint8_t start_index = floor(FRONT_LIGHT_MIDDLE - (FRONT_LIGHT_WIDTH / 2));
    uint8_t end_index = ceil(FRONT_LIGHT_MIDDLE + (FRONT_LIGHT_WIDTH / 2));
    for (uint8_t i = start_index; i <= end_index; i++) {
        uint8_t brightness = 255.0 * brightness_in_wipe(i, FRONT_LIGHT_MIDDLE, FRONT_LIGHT_WIDTH);
        // i=0 == i=MAX
        leds_front[N_LEDS_PER_STRIPE - N_LEDS_PER_SIDE - i] = CHSV(FRONT_LIGHT_HUE, 30, brightness);
        leds_front[N_LEDS_PER_SIDE + i] = CHSV(FRONT_LIGHT_HUE, 30, brightness);
    }

    start_index = floor(TAIL_LIGHT_MIDDLE - TAIL_LIGHT_WIDTH / 2);
    end_index = ceil(TAIL_LIGHT_MIDDLE + TAIL_LIGHT_WIDTH / 2);

    for (uint8_t i = start_index; i <= end_index; i++) {
        uint8_t brightness = 255.0 * brightness_in_wipe(i, TAIL_LIGHT_MIDDLE, FRONT_LIGHT_WIDTH);
        leds_back[N_LEDS_PER_STRIPE - N_LEDS_PER_SIDE - i] = CHSV(TAIL_LIGHTS_HUE, 255, brightness);
        leds_back[N_LEDS_PER_SIDE + i] = CHSV(TAIL_LIGHTS_HUE, 255, brightness);
    }
}

void turn_left() {
    println(F("turning left"));
    clear();
    reset_indicators();
}

void anim_left() {
    togglePin(Pins::BT_LEFT_RING);

    wipe(leds_front, false);
    wipe(leds_back, false);
}

void turn_right() {
    println(F("turning right"));
    clear();
    reset_indicators();
}

void anim_right() {
    togglePin(Pins::BT_RIGHT_RING);

    wipe(leds_front + N_LEDS_PER_SIDE, true);
    wipe(leds_back + N_LEDS_PER_SIDE, true);
}

void setup() {
    // max 24MHz, lower is more stable (which is good as we are carelessly routing long distances in
    // parallel) AtMega328 has 16MHz internal clock, be happy to get half of that
    FastLED
        .addLeds<SK9822, Pins::DATA_LEDS_FRONT, Pins::CLOCK_LEDS_FRONT, BGR, DATA_RATE_MHZ(8)>(
            leds_front, N_LEDS_PER_SIDE * 2)
        .setCorrection(TypicalLEDStrip);
    FastLED
        .addLeds<SK9822, Pins::DATA_LEDS_BACK, Pins::CLOCK_LEDS_BACK, BGR, DATA_RATE_MHZ(8)>(
            leds_back, N_LEDS_PER_SIDE * 2)
        .setCorrection(TypicalLEDStrip);

    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT__MA);
    FastLED.setBrightness(BRIGHTNESS);

    clear();
}

void hello_world() {
    for (uint8_t i = 0; i < N_LEDS_PER_SIDE; i++) {
        leds_front[i] = CRGB::White;
        leds_back[i] = CRGB::White;
        mirror(leds_front);
        mirror(leds_back);
        FastLED.show();
        FastLED.delay(100);
        leds_front[i] = CRGB::Black;
        leds_back[i] = CRGB::Black;
    }
    clear();
}

void sleep(uint32_t time_ms) { FastLED.delay(time_ms); }

} // namespace Lights