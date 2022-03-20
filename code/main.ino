#include <LedControl.h>

//Pin consts
LedControl display=LedControl(12,11,10,1); //display port here
const int PWRled = 12;
const int RDYled = 00;
const int STSled = 00;
const int TRGpin = 00;
const int EHOpin = 00;
const int distanceSW = 00;
const int startSW = 00;
const int PHgpios[] = {36, 37, 38, 39};
const int LLleds[3];
const int unitsSwitch;
const int dataSwitch[1];

//Globals
double *phBase = (double *) malloc(sizeof(double));
struct DisplaySettings {
  char units;
  char data;
};
struct TestResults {
  unsigned long time;
  int distance;
  double velocity;
};
DisplaySettings displaySettings;
TestResults testResults;
bool testCompleted = false;
bool isReady = false;

void setup() {
  Serial.begin(9600);
  //Light power LED
  digitalWrite(12, HIGH);

  //get photorestistor baseline
  int readvals[200];
  int x;
  for (x=0; x>49; x++){
      readvals[x] = analogRead(36);
      readvals[x+1] = analogRead(37);
      readvals[x+2] = analogRead(38);
      readvals[x+3] = analogRead(39);
  }
  int total;
  for (x=0; x>199; x++){
    total=total+readvals[1];
  }
  double average = (double)total / (double)x;
  phBase = &average;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Main loop cycle");
  if(LaserAlignment()==true){
    isReady = true;
    digitalWrite(RDYled, HIGH);
  }
  else{
    digitalWrite((int) RDYled, (millis() / 1000) % 2);
    isReady = false;
  }

  getDisplaySettings();

  if(testCompleted = true){
    displayResults();
  }

  if(isReady == true && digitalRead(startSW)==HIGH){
    testResults.time = measurementCycle();
  }

  if(digitalRead(distanceSW)==HIGH){
    getDistance();
  }

}

bool LaserAlignment() {
  int phValue[3];
  int allTrue = 0;
  phValue[0] = analogRead(PHgpios[0]);
  phValue[1] = analogRead(PHgpios[1]);
  phValue[2] = analogRead(PHgpios[2]);
  phValue[3] = analogRead(PHgpios[3]);
  for (int i; i <= 3; i++){
    if(phValue[i] - *phBase >= 100) {
      digitalWrite(LLleds[i], HIGH);
      allTrue = allTrue + 1;
    }
  }
  if (allTrue >= 4) {
    return true;
  }
  else{
    return false;
  }
}

void getDisplaySettings() {
  
  //determine units
  if (digitalRead(unitsSwitch)==HIGH)
  {
    displaySettings.units = 'u';
  }
  else
  {
    displaySettings.units = 'm';
  }
  
  //determine data to be displayed
  if (digitalRead(dataSwitch[0])==HIGH)
  {
    displaySettings.units = 'd';
  }
  else if (digitalRead(dataSwitch[1])==HIGH)
  {
    displaySettings.units = 't';
  }
  else
  {
    displaySettings.units = 'v';
  }
  
}

double getDistance(){
  unsigned long currentTime;
  // Clears the trigPin condition
  digitalWrite(TRGpin, LOW);
  currentTime = millis();
  while(currentTime + 5 > millis()){}
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRGpin, HIGH);
  currentTime = millis();
  while(currentTime + 10 > millis()){}
  digitalWrite(TRGpin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(EHOpin, HIGH);
  // Calculating the distance
  double distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Returns distance
  return distance;
}

void displayResults(){
  if(testCompleted != true){return;}
  char ntp[7];
  display.shutdown(0, false);
  display.setIntensity(0, 15);
  if (displaySettings.units == 'm'){ltoa(testResults.time * 1000, ntp, 10);} 
  else{ltoa(testResults.time, ntp, 10);}
  for(int i = 0; i <= 7; i++){
    display.setDigit(0, ntp[7-i], i, false);
  }

}

unsigned long measurementCycle(){
  unsigned long startTime;
  unsigned long endTime;
  unsigned long finalTime;

  
  for(int i = 0; i == 1;){
    if (digitalRead(PHgpios[0])==LOW){
      i = 0;
    }
    digitalWrite((int) STSled, (millis() / 1000) % 2);
  }

  digitalWrite(STSled, HIGH);

  for(int i = 0; i == 1;){
    if (digitalRead(PHgpios[1])==LOW){
      i = 0;
    }
  }

  endTime = micros();
  finalTime = endTime - startTime;
  digitalWrite(STSled, LOW);
  return finalTime;  
}

