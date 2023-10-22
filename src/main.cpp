#include <Arduino.h>
#include <time.h>

#define DEBUG

#include "shared/serialWrapper.h"
#include "pins.h"
#include "buttons.h"
#include "lights.h"
#include "power.h"
#include "sleep.h"
#include "states.h"

// 2.5ms seems to be lower limit atm
const uint16_t TARGET_LOOP_DURATION__MS = 5;

void setup() {
    // init first to deactivate unwanted output currents
    Pins::setup();
    Pins::clear_output();

    setupSerial(115200);
    println(F("Initializing..."));

    const time_t unixBuildTime = LAST_BUILD_TIME;
    print(F("Build date: "));
    printlnRaw(ctime(&unixBuildTime));  // this formatting is not great, but easy

    Buttons::setup();
    Lights::setup();
    Power::setup();

    println(F(".. Done!\nStarting IO tests..."));
    Pins::test_output();

    Lights::hello_world();

    println(F(".. Done!\n Ready for action!\n"));
}

void measure_looptime() {
    static uint32_t last_print = 0;
    static uint16_t call_counter = 0;
    call_counter++;

    uint32_t now = millis();
    uint16_t delta_t = now - last_print;

    if (delta_t > 5000) {
        last_print = now;
        print(F("Time-Delta "));
        printRaw(delta_t);
        print(F(" ms, Loop Calls "));
        printRaw(call_counter);
        print(F(" -> Average Looptime (ms): "));
        printlnRaw((float)delta_t / call_counter);
        call_counter = 0;
    }
}

void loop() {
    heartbeatSerial();
    Buttons::tick();
    Power::check_power();

    States::tick();

    // keep constant loop duration by aligning to target duration
    Lights::sleep(TARGET_LOOP_DURATION__MS - (millis() % TARGET_LOOP_DURATION__MS));
    measure_looptime();
}
