double *phBase = (double *) malloc(sizeof(double));

void setup() {
  //#include <FreeROTS\Arduino_FreeRTOS.h>

  Serial.begin(9600);
  //Light power LED
  digitalWrite(12, HIGH);

  //get photorestistor baseline
  int readvals[200];
  int x;
  for (x=0; x=49; x++){
      readvals[x] = analogRead(36);
      readvals[x+1] = analogRead(37);
      readvals[x+2] = analogRead(38);
      readvals[x+3] = analogRead(39);
  }
  int total;
  for (x=0; x=199; x++){
    total=total+readvals[1];
  }
  double average = (double)total / (double)x;
  phBase = &average;

  //Set ready LED blinking
  xTaskCreatePinnedToCore(
    blinkLed,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &BlinkLed);     /* Task handle to keep track of created task */
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Test");
}

TaskHandle_t BlinkLed;
void blinkLed(void *pvParameters){
  for(;;){digitalWrite(LedToBlink, (millis() / 1000) % 2);}
  }
