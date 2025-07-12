#include <Arduino.h>
#define irLED 16 //irLED on pin 2

//vars
unsigned long delayStart = 0;


// put function declarations here:
void high(int onTime){ //ontime is microseconds
  delayStart = micros();//start timer
  while ((micros() - delayStart) < onTime) //while current time minus when we started is less than specified ontime
  {
    digitalWrite(irLED,HIGH);
    delayMicroseconds(25);//40khz
    digitalWrite(irLED,LOW);
  }
  digitalWrite(irLED,LOW);//turn off once time has expired
} 


void LH(){//long high function
  high(1000);
  delayMicroseconds(500);
}

void SH(){
  high(500);

  delayMicroseconds(500);

}

void start(){
  high(2400);//2400 microseconds on 
  delayMicroseconds(600); //600 microseconds off

}

void one(){
  high(1200);
  delayMicroseconds(600);
}

void zero(){
  high(600);
  delayMicroseconds(600);
}

void on(){

  int data = 0b10101000000110;

  start();

  one();
  zero();
  one();
  zero();
  one();
  zero();
  zero();
  zero();
  zero();
  zero();
  zero();
  one();
  one();
  zero();
  zero();




};

void setup() {
  // put your setup code here, to run once:
  pinMode(irLED,OUTPUT);
}

void loop() {
  on();
  delay(20);//20ms delay before retrying
}

