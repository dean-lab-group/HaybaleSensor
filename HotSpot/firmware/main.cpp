#include "Particle.h"
#include "HaySense/HaySense.h"
#define publish_delay 1000

unsigned int lastPublish = 0;
HaySense hs;
String api_key = "HFHASFOUSMXDVZEL";
SYSTEM_THREAD(ENABLED);

int calibration_values(String command){
    Serial.println("I got, " + command);
    EEPROM.put(0, command);
}

void handler(const char *topic, const char *data) {
    Serial.println("received " + String(topic) + ": " + String(data));
    char name[24];
    strcpy(name, data);
    EEPROM.put(24, name);
}


void setup() {
    Particle.subscribe("particle/device/name", handler);
    Particle.publish("particle/device/name");
    Particle.function("calibrate", calibration_values);
    Serial.begin(9600);
}

void loop() {
    unsigned long now = millis();
    if ((now - lastPublish) < publish_delay) {
        return;
    }
    float temp_hz = hs.get_temperature_freq();
    float moist_hz = hs.get_moisture_freq();
    float temp = hs.get_temperature();
    float moist = hs.get_moisture();
    float battery = hs.get_battery_voltage();
    char name[24];
    EEPROM.get(24, name);
    String sensor_data = "{\"1\": \"" + String(temp) + "\", \"2\": \"" + String(temp_hz) + "\", \"3\": \"" + String(moist) + "\", \"4\": \"" + String(moist_hz) + "\", \"5\": \"" + String(battery) + "\", \"6\": \"" + System.deviceID() + "\", \"7\": \"" + name + "\", \"k\": \"" + api_key + "\"}";
    Particle.publish("thingSpeakWrite_hb", sensor_data, 60, PRIVATE);
    lastPublish = now;
    //if(Serial.isConnected()){
        // Serial.println(sensor_data);
        // Serial.printlnf("%f, %f", temp_hz, moist_hz);
    //}else{
    System.sleep(SLEEP_MODE_DEEP,300);
    //}
}
