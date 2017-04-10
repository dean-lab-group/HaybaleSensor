/* Haybale Wi-Fi sensor, V1.00
 *
 * This program times pulses on two input pins (D4 & D5)
 * and converts to a frequency. It then publishes the frequencies
 * to the Particle Cloud.
 *
 * __THSense__
 * This measures a variable frequency square wave. The pulseIn function
 * is used to measure the low and high pulses, and the two are added for
 * total period.
 *
 * __CapSense__
 * This measures a wave that is nearly square, but not enough for
 * pulseIn to function properly. Instead an interrupt is used to count
 * how many times the line goes high in 1 second, which itself is the
 * frequency.
 *
 * __End of program sleep__
 * The photon goes into deep sleep mode for 15 minutes after every program execution.
 * In this mode, when it restarts it does so from the very beginning. Only data
 * in EEPROM (not used in this program) is saved.
 *
 * Author: Andrew Muscha
 * email: andrew.muscha@auburn.edu
 * Last Updated: 10/31/2016
 */

SYSTEM_MODE(SEMI_AUTOMATIC); //disable automatic particle cloud connection

int BattVolt = A5; //naming individual pins
int CapSense = D4;
int THSense = D5;
int CtrlLine = D6; //power line for actual sensors

volatile long PulseCount = 0;  //used to count pulses on CapSense
volatile int LoopFlag = 0; //used to denote when a loop should be entered

double CapFreq = 0; //holds measure frequency
unsigned long THLowPeriod = 0; //holds low side of TH pulseIn
unsigned long THHighPeriod = 0; //holds high side of TH pulseIn
double THFreq = 0; //measures converted frequncy
double CurrentVoltage = 0; //holds measured battery voltage

String Variable = NULL; //string variables used for publishing data
String Data = NULL;
String mySSID;

unsigned int TimerPeriod = 1000; //timer period 1000 ms == 1 second

int SecFlag = 0; //flags when the 1second timer has elapsed
int FifteenMinutes = 900; //900 seconds = 15 minutes



void SetContinueFlag(void);

Timer timer(TimerPeriod, SetContinueFlag); //1 second timer for CapFreq calculation
										   //Calls SetContinueFlag on finish

void SetContinueFlag(){
  detachInterrupt(CapSense); //ends interrupts
  timer.stop(); //stops timer
  //RGB.color(255,255,0);
  SecFlag = 1; //set flag // sets flag to finish program
}

void CapISR(void){  //interrupt routine called every time a pulse is detected
//  RGB.color(0,0,255);
  if(!timer.isActive()){ //starts 2 second timer is it hasn't already started
    timer.startFromISR();
  }
  PulseCount++; //adds to pulse count
}

void setup() {
  //RGB.control(true);
  pinMode(CapSense, INPUT_PULLDOWN); //initialize pins
  pinMode(THSense, INPUT_PULLDOWN);
  pinMode(CtrlLine, OUTPUT);
  LoopFlag = 1; //initialize variable
  PulseCount = 0;
  SecFlag = 0;
  WiFi.on(); //turn wifi module on
  delay(10); //wait for it to power on

  WiFi.connect(WIFI_CONNECT_SKIP_LISTEN); //connect to saved networks
  while(WiFi.connecting()){} //wait for connection
  if(WiFi.ready()){ //if wifi is properly connected, connect to Particle Cloud
    if(Particle.connected() == false){
      Particle.connect();
      while(!Particle.connected() == false){}
    }
  }
}



void loop(){

  if(LoopFlag == 1){
    //RGB.color(255,0,0);
    pinResetFast(CtrlLine); //turns on sensors (sensors are active-low)
    attachInterrupt(CapSense, CapISR, FALLING); //activated interrupt for pulse counting
    LoopFlag = 0;
  }
  else if(SecFlag == 1){ //if 1 second timer finished (pulse count routine finished)
    //RGB.color(0,255,0);
    SecFlag = 0;
    LoopFlag = 1;

    CapFreq = PulseCount; //pulses = frequency
    PulseCount = 0;

    THLowPeriod = pulseIn(THSense, LOW); //get TH period lengths
    THHighPeriod = pulseIn(THSense, HIGH);
    pinSetFast(CtrlLine); //turn off sensors

    THFreq = (1 /  (double(THLowPeriod) + double(THHighPeriod))); //frequency = 1 / period
    THFreq = THFreq * 1000000;


	//Battery supplies at 4.5V, photon ADC reads up to 3.3 V. On-board resistor divider maps 4.5->3.3
	//	so photon can detect proper votlage range.
	CurrentVoltage = analogRead(BattVolt);
    CurrentVoltage = CurrentVoltage * ((3.3*2)/(4095*.914)); //reverses to 0-4.5V range for user

  //  RGB.color(0,255,255);*/
    if(Particle.connected()){

    mySSID = String(WiFi.SSID()); //get wifi network name
    Particle.publish(mySSID, NULL, 60, PRIVATE); //publishes name

    Variable = String("BatteryVolt"); //publish current battery voltage
    Data=String(CurrentVoltage);
    Particle.publish(Variable, Data, 60, PRIVATE);

    Variable = String("THFreq"); //publishes current TH frequency
    Data = String(THFreq);
    Particle.publish(Variable, Data, 60, PRIVATE);

    Variable = String("CapFreq"); //publishes current Cap frequency
    Data = String(CapFreq);
    Particle.publish(Variable, Data, 60, PRIVATE);
    LoopFlag = 1;
    delay(10000); //waits for data t finish transmission
  }


    //RGB.color(0,0,0);

    System.sleep(SLEEP_MODE_DEEP, FifteenMinutes); //go into deep sleep for 15 minutes
  }
}
