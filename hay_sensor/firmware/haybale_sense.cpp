#include "Particle.h"
#include "haybale_sense.h"

void power_on_sensors(){
    Log.trace("Powering on sensors");
    // Turns on sensors (sensors are active-low)
    pinResetFast(SENSOR_POWER_PIN);
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

bool setup_wifi(){
    Log.trace("Powering on WiFi.");
    WiFi.on();
    // Delays for 10 ms to wait for WiFi module to power on. I'm not sure why we do this.
    delay(10);
    Log.trace("Waiting for a WiFi connection.");
    WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
    while(WiFi.connecting()); //wait for connection
    if(WiFi.ready()){
        return true;
    }else{
        WiFi.off();
        return false;
    }
}

String getCoreID()
{
  String coreIdentifier = "";
  char id[12];
  memcpy(id, (char *)ID1, 12);
  char hex_digit;
  for (int i = 0; i < 12; ++i)
  {
    hex_digit = 48 + (id[i] >> 4);
    if (57 < hex_digit)
     hex_digit += 39;
     coreIdentifier = coreIdentifier + hex_digit;
    hex_digit = 48 + (id[i] & 0xf);
   if (57 < hex_digit)
     hex_digit += 39;
   coreIdentifier = coreIdentifier + hex_digit;
 }
 return coreIdentifier;
}


/*Sensor::Sensor(void){
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
*/
