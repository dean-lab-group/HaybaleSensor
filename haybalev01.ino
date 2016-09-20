/* Haybale Wi-Fi sensor, V0.01
 *
 * This program times pulses on two input pins (D3 & D2)
 * and converts to a frequency. It then publishes the frequencies
 * to the Particle Cloud.
 * 
 * Current Update Frequency : 1 /hour
 * Current Timer Frequency : 1 /minute
 *
 *
 * Future work: see TODO.txt
 *
 * Author: Andrew Muscha
 * email: andrew.muscha@auburn.edu
 * Last Updated: 9/20/2016
 */


int CapSense = D3;
int THSense = D2;
double CapHalfPeriod = 0; 
double CapPeriod = 0;
double CapFreq = 0;
double THHalfPeriod = 0;
double THPeriod = 0;
double THFreq = 0;
String Variable = NULL;
String Data = NULL;


int CurrentHour = 25;
int NextHour = 25;

int UpdateFlag = 0;
int MinuteInterval = 60000; //60000 milliseconds in a minute

void check_if_hour_elapsed();
void FirstCheck();

Timer timer(MinuteInterval, check_if_hour_elapsed); //timer used for periodic checks

void setup() {
  pinMode(CapSense, INPUT_PULLDOWN);
  pinMode(THSense, INPUT_PULLDOWN);

  CurrentHour = Time.hour(); //establish current time
  if(CurrentHour >= 23){  //if-else statement prevents non-existent hours
    NextHour = 0;
  }else{
    NextHour = CurrentHour + 1;
  }

  Particle.variable("Cap Freq", CapFreq); //two cloud variables
  Particle.variable("Temp Freq", THFreq); //Markus, you can try fetching these using api
  
  FirstCheck(); //initializes current temp and cap values
  
  Particle.publish("Setup complete", NULL, 60, PRIVATE); //setup function finishes out

  timer.start(); 
}

void loop() {
  if(UpdateFlag){  //if values were updated
    timer.stop(); //prevents timer from intiating more checks while updates are processed
    UpdateFlag = 0; //clear flag
    CapFreq = 1 /  CapPeriod; //frequency = 1/periodic
    THFreq = 1 / THPeriod;

    Variable = String("CapFreq"); //preparing data as strings to be published
    Data = String(CapFreq);
    Particle.publish(Variable, Data, 60, PRIVATE); //publish current data

    Variable = String("THFreq");
    Data = String(THFreq);
    Particle.publish(Variable, Data, 60, PRIVATE);

    timer.start(); //restart timer for periodic updates
  }
}

void check_if_hour_elapsed(){ //called by timer to check if it's time to update
  CurrentHour = Time.hour(); //update current time
  if(CurrentHour == NextHour){ //check if hour elapsed and set flag if it has
    UpdateFlag = 1;
    if(CurrentHour >= 23){ //if-else statement prevents non-existent hours
      NextHour = 0;
    }else{
      NextHour = CurrentHour + 1;
    }
    CapHalfPeriod = pulseIn(CapSense, HIGH); //pulseIn times a low or high pulse, aka half of a period of a square wave
    CapPeriod = 2 * CapHalfPeriod; //double pulse length
    CapPeriod = CapPeriod * .000001; //pulseIn returns value in milliseconds; convert to seconds
    THHalfPeriod = pulseIn(THSense, HIGH);
    THPeriod = 2 * THHalfPeriod;
    THPeriod = THPeriod * .000001;
  }
}

void FirstCheck(){
  CapHalfPeriod = pulseIn(CapSense, HIGH); //pulseIn times a low or high pulse, aka half of a period of a square wave
  CapPeriod = 2 * CapHalfPeriod; //double pulse length
  CapPeriod = CapPeriod * .000001; //pulseIn returns value in milliseconds; convert to seconds
  THHalfPeriod = pulseIn(THSense, HIGH);
  THPeriod = 2 * THHalfPeriod;
  THPeriod = THPeriod * .000001;

  CapFreq = 1 /  CapPeriod; //frequency = 1 / period
  THFreq = 1 / THPeriod;

  Variable = String("CapFreq"); //preparing data as strings to be published
  Data = String(CapFreq);
  Particle.publish(Variable, Data, 60, PRIVATE); //publish current data

  Variable = String("THFreq");
  Data = String(THFreq);
  Particle.publish(Variable, Data, 60, PRIVATE);

}
