//
// Created by Markus Kreitzer on 8/11/17.
//

#ifndef FIRMWARE_HAYBALE_SENSE_H
#define FIRMWARE_HAYBALE_SENSE_H

// All my defs
#define BATTERY_PING_VOLTS A5  //naming individual pins
#define CAPACITOR_PIN_VOLTAGE D4
#define TEMPERATURE_PIN_VOLTS D5
#define SENSOR_POWER_PIN_VOLTS D6  //power line for actual sensors
#define POWER_ON_DELAY 10

// Calibration Data
// These will be #define's for now, but eventually we need to build a calibration scheme.
// Sensor "Haybale 4"
// Calibration
// temp = 0.00324 * temp_freq + -2.15929*voltage + 24.70709
#define TEMP_FREQ_COEF 0.00324
#define TEMP_VOLT_COEF -2.15929
#define TEMP_SHIFT 24.70709

// moist = {xxx}
#define MOIST_FREQ_COEF 0.00008
#define MOIST_VOLT_COEF 0.0
#define MOIST_SHIFT 0.0
#endif //FIRMWARE_HAYBALE_SENSE_H


class Sensor {
private:
    void set_continue_flag(void);
    void detect_pulse(void);
    Timer timer;
    volatile long pulse_count;    // used to count pulses on CapSense
    volatile int loop_flag;       // used to denote when a loop should be entered
    double cap_freq;              // holds measure frequency
    unsigned long th_low_period;  // holds low side of TH pulseIn
    unsigned long th_high_period; // holds high side of TH pulseIn
    double th_freq;               // measures converted frequency
    double current_voltage;       // holds measured battery voltage
    int sec_flag;                 // flags when the 1second timer has elapsed
    unsigned int timer_period;    // timer period 1000 ms == 1 second

public:
    Sensor(void);
    double get_temperature_freq(void);
    double get_moisture_freq(void);
    double temperature;
    double moisture;
};
