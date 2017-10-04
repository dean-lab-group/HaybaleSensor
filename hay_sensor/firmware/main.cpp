#include "Particle.h"
#include "haybale_sense.h"
#include "convert.h"
#include "CalibrationManager.h"
#include "FreqPeriodCounter.h"

//SYSTEM_MODE(SEMI_AUTOMATIC); //disable automatic particle cloud connection
SYSTEM_MODE(MANUAL); //disable automatic particle cloud connection
//SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(115200, LOG_LEVEL_ERROR);
FreqPeriodCounter counter(MOISTURE_PIN, micros, 0);
CalibrationManager myCalibrationManager;

// ***************************************
// Functions
// ***************************************
void counterISR()
{
	counter.poll();
}

float get_moisture_freq(void){
    while(!counter.ready());
    return counter.hertz();
}

void setup(void){
        Serial.begin(115200);
        delay(SERIAL_DELAY);
        Serial.println("Serial Activated");
        Log.info("Entering setup.");
        Log.info("The unique ID of this core is:");
        Log.info(getCoreID());

        Log.trace("Setting up pins!");
        pinMode(MOISTURE_PIN, INPUT_PULLDOWN);
        pinMode(TEMPERATURE_PIN, INPUT_PULLDOWN);
        pinMode(SENSOR_POWER_PIN, OUTPUT);
        attachInterrupt(MOISTURE_PIN, counterISR, CHANGE);

        // If we get no wifi connection, there is no point in further wasting
        // battery power so we'll just go ahead and power the whole system down
        // for a specificied time. See below
        #ifndef CALIBRATION_MODE
        Log.info("Skipping WiFi activation");
        #else
        if(waitFor(setup_wifi, WIFI_TIMEOUT)){
            Log.info("Connected to WiFi");
            //Log.info("Powering on sensors");
            //power_on_sensors();
            Particle.connect();
            if(Particle.connected()){
                Log.info("Particle is connected to the cloud!");
                Particle.process();
            }else{
                Log.error("No cloud connection available.");
            }
        }else{
            Log.info("No Wifi connection. Going to sleep.");
            //System.sleep(SLEEP_MODE_DEEP, DEEP_SLEEP_TIME);
        }
        #endif
}

void loop(void){
    Log.trace("Entering loop");
    // Get stuff. I need to move the stuff below to a class...
    power_on_sensors();
    float battery_voltage = get_battery_voltage();
    Log.trace(String(battery_voltage));
    float temperature_frequency = get_temperature_freq();
    Log.trace(String(temperature_frequency));
    float moisture_frequency = get_moisture_freq();
    Log.trace(String(moisture_frequency));
    Log.trace("Powering down");
    //power_down();
    Serial.print(temperature_frequency);
    Serial.print(", ");
    Serial.println(moisture_frequency);
    delay(500);
}
