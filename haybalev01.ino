/* Haybale Wi-Fi sensor, V0.01
 *
 * This program times pulses on two input pins (D3 & D2)
 * and converts to a frequency. It then publishes the frequencies
 * to the Particle Cloud.
 *
 * Current Update Frequency :  60/hour
 * Current Timer Frequency : every 0.5 minutes
 *
 * Currently using sleep mode @ 40mA
 *
 * Future work: see TODO.txt
 *
 * Author: Andrew Muscha
 * email: andrew.muscha@auburn.edu
 * Last Updated: 9/20/2016
 */
long SleepTimeInSeconds = 86400;

int BattVolt = A5;
int CapSense = D3;
int THSense = D5;
int CtrlLine = D6;
int error = D7;

volatile long PulseCount = 0;
volatile int LoopFlag = 0;
volatile int PulseFlag = 0;

unsigned long CapLowPeriod = 0;
unsigned long CapHighPeriod = 0;
double CapFreq = 0;
unsigned long THLowPeriod = 0;
unsigned long THHighPeriod = 0;
double THFreq = 0;
double CurrentVoltage = 0;

String Variable = NULL;
String Data = NULL;

unsigned int TimerPeriod = 1000;

int UpdateFlag = 0;
int SecFlag = 0;
int FiveMinuteInterval = 30000; //60000 milliseconds in a minute; 300000 in 5


void SetContinueFlag(void);

Timer timer(TimerPeriod, SetContinueFlag);

void SetContinueFlag(){
  noInterrupts();
  timer.stop();
  RGB.color(255,255,0);
  SecFlag = 1; //set flag
}

void CapISR(void){
  RGB.color(0,0,255);
  //delayMicroseconds(8);
  //if(pinReadFast(CapSense) == LOW){
    if(!timer.isActive()){
      timer.startFromISR();
    }
    PulseCount++;
  //}
}

void setup() {
  RGB.control(true);
  pinMode(CapSense, INPUT_PULLDOWN);
  pinMode(THSense, INPUT_PULLDOWN);
  pinMode(CtrlLine, OUTPUT);
  pinMode(error, OUTPUT);
  attachInterrupt(CapSense, CapISR, FALLING);
  LoopFlag = 1;
  PulseCount = 0;
  SecFlag = 0;
}



void loop(){

  if(LoopFlag == 1){
    RGB.color(255,0,0);
    pinResetFast(CtrlLine);
    interrupts();
    LoopFlag = 0;
  }
  else if(SecFlag == 1){
    RGB.color(0,255,0);
    SecFlag = 0;
    //  LoopFlag = 1;
    /*Variable = String("Pulses");
    Data = String(PulseCount);
    Particle.publish(Variable, Data, 60, PRIVATE);*/

    CapFreq = PulseCount;//double(PulseCount) / 2.007; //pulses = frequency
    PulseCount = 0;

    //CapLowPeriod = pulseIn(CapSense, LOW); //pulseIn times a low or high pulse, aka half of a period of a square wave
    //CapHighPeriod = pulseIn(CapSense, HIGH);
    delay(1000);

    THLowPeriod = pulseIn(THSense, LOW);
    THHighPeriod = pulseIn(THSense, HIGH);

    /*CapLowPeriod = pulseIn(CapSense, LOW);
    CapHighPeriod = pulseIn(CapSense, HIGH);*/

    pinSetFast(CtrlLine); //turn off sensors


    THFreq = (1 /  (double(THLowPeriod) + double(THHighPeriod))); //frequency = 1 / period
    //CapFreq = CapFreq * 1000000;
    THFreq = THFreq * 1000000;


    CurrentVoltage = analogRead(BattVolt);
    CurrentVoltage = CurrentVoltage * ((3.3*2)/(4095*.914));

    RGB.color(0,255,255);
    while(!Particle.connected()){ }

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


    RGB.color(0,0,0);
    System.sleep(SLEEP_MODE_DEEP, 10);
  }
}
