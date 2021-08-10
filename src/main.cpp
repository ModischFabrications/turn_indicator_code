#include <Arduino.h>

#include "buttons.h"
#include "lights.h"
#include "pins.h"
#include "power.h"
#include "sleep.h"
#include "states.h"

// 2.5ms seems to be lower limit atm
const uint16_t TARGET_LOOP_DURATION__MS = 5;

void setup() {
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

    Serial.println(F("Ready for action!"));
}

void measure_looptime() {
    static uint32_t last_print = 0;
    static uint16_t call_counter = 0;
    call_counter++;

    uint32_t now = millis();
    uint16_t delta_t = now - last_print;

    if (delta_t > 5000) {
        last_print = now;
        Serial.print(F("Time-Delta "));
        Serial.print(delta_t);
        Serial.print(F(" ms, Loop Calls "));
        Serial.print(call_counter);
        Serial.print(F(" -> Average Looptime (ms): "));
        Serial.println((float)delta_t / call_counter);
        call_counter = 0;
    }
}

void loop() {
    Buttons::tick();
    Power::check_power();

    States::tick();

    // keep constant loop duration by aligning to target duration
    Lights::sleep(TARGET_LOOP_DURATION__MS - (millis() % TARGET_LOOP_DURATION__MS));
    measure_looptime();
}
