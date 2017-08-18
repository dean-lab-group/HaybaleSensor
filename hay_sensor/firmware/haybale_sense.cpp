#include "Particle.h"
#include "haybale_sense.h"

bool setup_wifi(){
    WiFi.on();
    // Delays for 10 ms to wait for WiFi module to power on. I'm not sure why we do this.
    delay(10);
    while(WiFi.connecting()){} //wait for connection
    WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
    if(WiFi.ready()){ //if wifi is properly connected, connect to Particle Cloud
        return true;
    }else{
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
