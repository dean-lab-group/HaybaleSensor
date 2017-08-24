#include "Particle.h"
#include "haybale_sense.h"
#include "convert.h"
#include "Calibrator.h"

//SYSTEM_MODE(SEMI_AUTOMATIC); //disable automatic particle cloud connection
SYSTEM_MODE(MANUAL); //disable automatic particle cloud connection
SerialLogHandler logHandler;
Calibrator myCalibrator;

// Global Variables
volatile long pulse_count;
boolean moisture_timer_complete;
bool first_run = true;
unsigned int moisture_frequency = 0;
float battery_voltage;

// Test HB3


// ***************************************
// Functions
// ***************************************
void stop_moisture_freq_measurement(void);

// 1 second timer for CapFreq calculation
//Calls SetContinueFlag on finish
Timer moisture_frequency_timer(TIMER_PERIOD, stop_moisture_freq_measurement);

void stop_moisture_freq_measurement(){
  detachInterrupt(MOISTURE_PIN); //ends interrupts
  moisture_frequency_timer.stop(); //stops timer

  // We set the below flag to indicate that the time ran and we concluded
  // measuring the frequecy of the moisture pin wave.
  moisture_timer_complete = true; //set flag // sets flag to finish program
  moisture_frequency = pulse_count;
  pulse_count = 0;
}

void detect_pulse_ISR(void) {
    // Interrupt routine called every time a pulse is detected.
    // If the moisture timer has started, we start the timer when the ISR is
    // triggered. Technically only "FromISR" type functions are supposed to be
    // called but this seems to have worked in the past before I took this
    // project over.
    if (!moisture_frequency_timer.isActive()) {
        moisture_frequency_timer.startFromISR();
        pulse_count = 0;
        moisture_timer_complete = false;
    }
    // Increment global pulse_count with the number of pulses detected.
    // Since the timer runs for one second, we can say the pulse count is the
    // frequency.
    pulse_count++;
}

void power_on_sensors(void){
    // Turns on sensors (sensors are active-low)
    pinResetFast(SENSOR_POWER_PIN);
    // Activated interrupt for pulse counting
    attachInterrupt(MOISTURE_PIN, detect_pulse_ISR, FALLING);
}

float get_moisture_freq(void){
    Log.trace("Getting temperature frequency.");
    power_on_sensors();
    while(!moisture_timer_complete);
    return moisture_frequency;
}

float get_battery_voltage(void){
    Log.trace("Getting battery voltage.");
    // Funky math Andrew M. worked out to convert the analogRead value to the
    // proper battery voltage value.
    return (float)analogRead(BATTERY_PIN) * ((3.3*2.0)/(4095.0*0.914));
}

float get_temperature_freq(void){
    Log.trace("Getting temperature frequency.");
    unsigned long temp_period_low = pulseIn(TEMPERATURE_PIN, LOW);
    unsigned long temp_period_high = pulseIn(TEMPERATURE_PIN, HIGH);
    return (1000000.0 * (1.0/(float)(temp_period_low + temp_period_high)));
}

void power_down(void){
    Log.trace("Powering down.");
    pinSetFast(SENSOR_POWER_PIN); //turn off sensors
    System.sleep(SLEEP_MODE_DEEP, DEEP_SLEEP_TIME);
}

void setup(void){
        Log.info("Entering setup.");
        Log.info("The unique ID of this core is ");
        Log.info(getCoreID());

        // If we get no wifi connection, there is no point in further wasting
        // battery power so we'll just go ahead and power the whole system down
        // for a specificied time. See below
        if(waitFor(setup_wifi, WIFI_TIMEOUT)){
            Log.info("Connected to wifi");
            Log.info("Powering on sensors");
            power_on_sensors();
        }else{
            Log.info("No Wifi connection. Going to sleep.");
            System.sleep(SLEEP_MODE_DEEP, DEEP_SLEEP_TIME);
        }
}

void loop(void){
    Log.trace("Entering loop");
    // Get stuff. I need to move the stuff below to a class...
    float battery_voltage = get_battery_voltage();
    float moisture_frequency = get_moisture_freq();
    float temperature_frequency = get_temperature_freq();
    Log.trace("Sensor Values:");
    Log.trace(String(battery_voltage));
    Log.trace(String(moisture_frequency));
    Log.trace(String(temperature_frequency));
    Log.trace("Powering down");
    power_down();
}
