#include "Particle.h"

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

void power_down(void){
    Log.trace("Powering down.");
    System.sleep(SLEEP_MODE_DEEP, DEEP_SLEEP_TIME);
}
