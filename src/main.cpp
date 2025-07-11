#include <Arduino.h>
#define irLED 2 //irLED on pin 2
// put function declarations here:
void on(){
  start();
  LH();
  SH();
  LH();
  SH();
  LH();
  SH();
  SH();
  SH();
  SH();
  SH();
  SH();
  LH();
  LH();
  SH();
  SH();

};

void LH(){//long high function
  digitalWrite(irLED,HIGH);
  delay(1);//delay 1 ms
  digitalWrite(irLED,LOW);
}

void SH(){
  digitalWrite(irLED,HIGH);
  delayMicroseconds(500);//delay 500 microseconds
  digitalWrite(irLED,LOW);
}

void start(){
  digitalWrite(irLED,HIGH);
  delay(2);//delay 500 microseconds
  digitalWrite(irLED,LOW);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(irLED,OUTPUT);
}

void loop() {
  on();
  delay(100);
}

