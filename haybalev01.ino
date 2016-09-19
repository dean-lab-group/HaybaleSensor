int CapSense = D7;
int THSense = D6;
double CapPeriod = 0;
double CapFreq = 0;
double THPeriod = 0;
double THFreq = 0;
String Variable = NULL;
String Data = NULL;


int CurrentHour = 25;
int NextHour = 25;

int UpdateFlag = 0;
int MinuteInterval = 60000;

void check_if_hour_elapsed();
void FirstCheck();

Timer timer(MinuteInterval, check_if_hour_elapsed);

void setup() {
  pinMode(CapSense, INPUT_PULLDOWN);
  pinMode(THSense, INPUT_PULLDOWN);

  CurrentHour = Time.hour();
  if(CurrentHour >= 23){
    NextHour = 0;
  }else{
    NextHour = CurrentHour + 1;
  }

  Particle.publish("Setup complete", NULL, 60, PRIVATE);
  Particle.variable("Cap Freq", CapFreq);
  Particle.variable("Temp Freq", THFreq);

  FirstCheck();

  timer.start();
}

void loop() {
  if(UpdateFlag){
    timer.stop();
    UpdateFlag = 0;
    CapFreq = 1 /  CapPeriod;
    THFreq = 1 / THPeriod;

    Variable = String("CapFreq");
    Data = String(CapFreq);
    Particle.publish(Variable, Data, 60, PRIVATE);

    Variable = String("THFreq");
    Data = String(THFreq);
    Particle.publish(Variable, Data, 60, PRIVATE);

    timer.start();
  }
}

void check_if_hour_elapsed(){
  CurrentHour = Time.hour();
  if(CurrentHour == NextHour){
    UpdateFlag = 1;
    if(CurrentHour >= 23){
      NextHour = 0;
    }else{
      NextHour = CurrentHour + 1;
    }
    CapPeriod = pulseIn(CapSense, HIGH);
    CapPeriod = 2 * CapPeriod;
    THPeriod = pulseIn(THSense, HIGH);
    THPeriod = 2 * THPeriod;
  }
}

void FirstCheck(){
  CapPeriod = pulseIn(CapSense, HIGH);
  CapPeriod = 2 * CapPeriod;
  THPeriod = pulseIn(THSense, HIGH);
  THPeriod = 2 * THPeriod;

  CapFreq = 1 /  CapPeriod;
  THFreq = 1 / THPeriod;

  Variable = String("CapFreq");
  Data = String(CapPeriod);
  Particle.publish(Variable, Data, 60, PRIVATE);

  Variable = String("THFreq");
  Data = String(THPeriod);
  Particle.publish(Variable, Data, 60, PRIVATE);

}
