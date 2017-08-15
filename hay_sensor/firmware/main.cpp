#include "Particle.h"
#include "haybale_sense.h"

void setup(void)
{
  Serial.begin(115200);
}

void loop(void)
{
  if (WiFi.listening())
  {
    // In listening mode, running SoftAP
    return;
  }
}
