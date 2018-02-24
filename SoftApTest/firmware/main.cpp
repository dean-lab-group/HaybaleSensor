#pragma SPARK_NO_PREPROCESSOR
#include "Particle.h"
#include "webpage.h"
SYSTEM_THREAD(ENABLED);
STARTUP(softap_set_application_page_handler(Main, nullptr));

void setup() {
    WiFi.listen();
    //hs.power_on_sensors();
    Serial.begin();
    delay(1000);
    Serial.print("freeMemory = ");
    Serial.println(System.freeMemory());
    delay(1000);

}

void loop() {
}
