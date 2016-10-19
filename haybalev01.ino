/* Haybale Wi-Fi sensor, V0.01
 *
 * This program times pulses on two input pins (D4 & D5)
 * and converts to a frequency. It then publishes the frequencies
 * to the Particle Cloud.
 *
 *
 * Future work: see TODO.txt
 *
 * Author: Andrew Muscha
 * email: andrew.muscha@auburn.edu
 * Last Updated: 10/18/2016
 */

SYSTEM_MODE(SEMI_AUTOMATIC);
long SleepTimeInSeconds = 86400;

int BattVolt = A5;
int CapSense = D4;
int THSense = D5;
int CtrlLine = D6;
int error = D7;

volatile long PulseCount = 0;
volatile int LoopFlag = 0;
volatile int PulseFlag = 0;

double CapFreq = 0;
unsigned long THLowPeriod = 0;
unsigned long THHighPeriod = 0;
unsigned long CapLowPeriod = 0;
unsigned long CapHighPeriod = 0;
double THFreq = 0;
double CurrentVoltage = 0;

String Variable = NULL;
String Data = NULL;
String mySSID;

unsigned int TimerPeriod = 1000;

int UpdateFlag = 0;
int SecFlag = 0;
int FifteenMinutes = 900;
long offset = 0;

WiFiAccessPoint aps[20];
int found;



void SetContinueFlag(void);

Timer timer(TimerPeriod, SetContinueFlag);

void SetContinueFlag(){
  detachInterrupt(CapSense);
  timer.stop();
  //RGB.color(255,255,0);
  SecFlag = 1; //set flag
}

void CapISR(void){
//  RGB.color(0,0,255);
  //delayMicroseconds(8);
  //if(pinReadFast(CapSense) == LOW){
    if(!timer.isActive()){
      timer.startFromISR();
    }
    PulseCount++;
  //}
}

void setup() {
  //noInterrupts();
  //RGB.control(true);
  pinMode(CapSense, INPUT_PULLDOWN);
  pinMode(THSense, INPUT_PULLDOWN);
  pinMode(CtrlLine, OUTPUT);
  pinMode(error, OUTPUT);
  LoopFlag = 1;
  PulseCount = 0;
  SecFlag = 0;
  WiFi.on();
  delay(10);

  WiFi.connect(WIFI_CONNECT_SKIP_LISTEN);
//  delay(3000);
  while(WiFi.connecting()){}
  if(WiFi.ready()){
    if(Particle.connected() == false){
      Particle.connect();
      while(!Particle.connected() == false){}
    }
  }
}



void loop(){

  if(LoopFlag == 1){
    //RGB.color(255,0,0);
    pinResetFast(CtrlLine);
    attachInterrupt(CapSense, CapISR, FALLING);
    LoopFlag = 0;
  }
  else if(SecFlag == 1){
    //RGB.color(0,255,0);
    SecFlag = 0;
    LoopFlag = 1;

    CapFreq = PulseCount; //pulses = frequency
    PulseCount = 0;

    THLowPeriod = pulseIn(THSense, LOW);
    THHighPeriod = pulseIn(THSense, HIGH);
    pinSetFast(CtrlLine); //turn off sensors

    THFreq = (1 /  (double(THLowPeriod) + double(THHighPeriod))); //frequency = 1 / period
    THFreq = THFreq * 1000000;

    CurrentVoltage = analogRead(BattVolt);
    CurrentVoltage = CurrentVoltage * ((3.3*2)/(4095*.914));

  //  RGB.color(0,255,255);*/
    if(Particle.connected()){

    mySSID = String(WiFi.SSID());
    Particle.publish(mySSID, NULL, 60, PRIVATE);

    Variable = String("BatteryVolt");
    Data=String(CurrentVoltage);
    Particle.publish(Variable, Data, 60, PRIVATE);

    Variable = String("THFreq");
    Data = String(THFreq);
    Particle.publish(Variable, Data, 60, PRIVATE);

    Variable = String("CapFreq");
    Data = String(CapFreq);
    Particle.publish(Variable, Data, 60, PRIVATE);
    LoopFlag = 1;
    delay(10000);
  }


    //RGB.color(0,0,0);
    offset = millis();
    offset = offset *.001;
    offset = FifteenMinutes - offset;
  //  offset = (long)offset;

    System.sleep(SLEEP_MODE_DEEP, offset);
  }
}
