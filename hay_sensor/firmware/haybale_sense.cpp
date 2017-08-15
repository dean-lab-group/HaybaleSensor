#include "Particle.h"
#include "haybale_sense.h"

Sensor::Sensor(void){
    this->temperature;
    this->moisture;
    this->pulse_count = 0
    pinMode(CAPACITOR_PIN_VOLTAGE, INPUT_PULLDOWN);
    pinMode(TEMPERATURE_PIN_VOLTS, INPUT_PULLDOWN);
    pinMode(SENSOR_POWER_PIN_VOLTS, OUTPUT);
}

//1 second timer for capactance frequency calculation
//Calls SetContinueFlag on finish
Timer Sensor::timer(timer_period, set_continue_flag);

// Get the moisture frequency to determine what the percentage moisture is.
double Sensor::get_moisture_freq(void){
    return this->cap_freq;
}

// Get the temperature frequency to determine what the temperature is.
double Sensor::get_temperature_freq(void)
{
    return this->temp_freq;
}

void set_continue_flag(void)
{
    detachInterrupt(CAPACITOR_PIN_VOLTAGE); //ends interrupts
    this->timer.stop(); //stops timer
    sec_flag = 1;
    //set flag
    // sets flag to finish program
}

void detect_pulse(void) {  //interrupt routine called every time a pulse is detected
    if (!timer.isActive()) { //starts 2 second timer is it hasn't already started
        timer.startFromISR();
    }
    this->pulse_count++; //adds to pulse count
}
