#include <LedControl.h>

//Pin consts
LedControl display=LedControl(12,11,10,1); //display port here
//const int PWRled = 00;
const int RDYled = 12;
const int STSled = 13;
const int TRGpin = 16;
const int EHOpin = 17;
const int startSW = 19;
const int PHgpios[] = {36, 37, 38};
const int LLleds[] = {21, 22, 23, 25};
const int unitsSwitch = 26;
const int dataSwitch = 27;

//Globals
struct DisplaySettings {
  char units;
  char data;
};
struct TestResults {
  unsigned long firstTime;
  unsigned long secondTime;
  unsigned long totalTime;
  double firstVelocity;
  double secondVelocity;
  double totalVelocity;
};
DisplaySettings displaySettings;
TestResults testResults;
bool testCompleted = false;
bool isReady = false;

void setup() {
  Serial.begin(9600);
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
    measurementCycle();
  }

}

bool LaserAlignment() {
  int phValue[2];
  int allTrue = 0;
  phValue[0] = digitalRead(PHgpios[0]);
  phValue[1] = digitalRead(PHgpios[1]);
  phValue[2] = digitalRead(PHgpios[2]);
  if ((phValue[0] == HIGH) && (phValue[1] == HIGH) && (phValue[2] == HIGH)) {
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
  else if (digitalRead(dataSwitch)==HIGH)
  {
    displaySettings.data = 't';
  }
  else
  {
    displaySettings.data = 'v';
  }
  
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

void measurementCycle(){
  unsigned long startTime;
  unsigned long midTime;
  unsigned long endTime;
  unsigned long finalTime;

  
  for(int i = 0; i == 1;){
    if (digitalRead(PHgpios[0])==LOW){
      i = 1;
    }
    startTime = micros();
    digitalWrite((int) STSled, (millis() / 1000) % 2);
  }

  digitalWrite(STSled, HIGH);

    for(int i = 0; i == 1;){
    if (digitalRead(PHgpios[1])==LOW){
      midTime = micros();
      i = 1;
    }
    digitalWrite((int) STSled, (millis() / 1000) % 2);
  }

  for(int i = 0; i == 1;){
    if (digitalRead(PHgpios[2])==LOW){
      endTime = micros();
      i = 1;
    }
  }

  testResults.firstTime = midTime - startTime
  testResults.secondTime = endTime - midTime
  testResults.totalTime = endTime - startTime
  digitalWrite(STSled, LOW);
}

