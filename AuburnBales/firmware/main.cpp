#include "Particle.h"
#include "HaySense/HaySense.h"
#include "api_keys.h"
SYSTEM_THREAD(ENABLED);

unsigned int lastPublish = 0;
// Delay between each loop publishing data.
#define publish_delay 10000

HaySense hs;

struct Calibration {
   float moisture[4];
   float temperature[2];
};

int calibration_values(String command){
    Serial.print("I got, " + command);
    //
    //float boo = sscanf("%f;%f;%f;%f;%f;%f\n", command.asCh);

    // Serial.println(command.moisture[0]);
    // Serial.println(command.moisture[1]);
    // Serial.println(command.moisture[2]);
    // Serial.println(command.moisture[3]);
    //Serial.printlnf("%f %f %f %f %f %f", command);
    //EEPROM.put(0, command);
    Serial.println(sizeof(command)*command.length());
    return sizeof(command);
}

void handler(const char *topic, const char *data) {
    Serial.println("received " + String(topic) + ": " + String(data));
    // char name[24];
    // strcpy(name, data);
    // EEPROM.put(24, name);
}

Converter conv(moist_constants,temp_constants);

void setup() {
    //Particle.subscribe("particle/device/name", handler);
    //Particle.publish("particle/device/name");
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
    float temp = conv.GetTemperature(temp_hz);
    float moist = conv.GetMoisture(moist_hz, temp_hz);
    float battery = hs.get_battery_voltage();
    char name[24];
    //EEPROM.get(24, name);
    String sensor_data = "{\"1\": \"" + String(temp) + "\", \"2\": \"" + String(temp_hz) + "\", \"3\": \"" + String(moist) + "\", \"4\": \"" + String(moist_hz) + "\", \"5\": \"" + String(battery) + "\", \"6\": \"" + System.deviceID() + "\", \"7\": \"" + name + "\", \"k\": \"" + api_key + "\"}";
    Particle.publish("thingSpeakWrite_hb", sensor_data, 60, PRIVATE);
    lastPublish = now;
    if(Serial.isConnected()){
      Serial.println("Publishing:")
      Serial.println(sensor_data);
      //Serial.printlnf("%f, %f", temp_hz, moist_hz);
    }else{
      System.sleep(SLEEP_MODE_DEEP, 300);
    }
}
