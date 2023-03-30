#include <SoftwareSerial.h>
#include <TinyGPS++.h>


int count = 0;

void setup() {
  pinMode(PC7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PC7), callback, FALLING); 
}


void loop() {
  Serial.println("counted:");
  Serial.println(count)
  delay(5000);
}



void callback() {
  count++;
}