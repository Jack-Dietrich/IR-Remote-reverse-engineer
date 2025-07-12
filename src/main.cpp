#include <Arduino.h>
#define irLED 16 //irLED on pin 2
#include "BluetoothSerial.h" //for bluetooth connectivity

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


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

  int data = 0b101010000001100;
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
  Serial.begin(115200);

  //bluetooth setup
  SerialBT.begin("ESP32 Testing");
  Serial.println("Bluetooth started");
  SerialBT.println("Test line");

}

void loop() {

  if(SerialBT.available()){//if we recieve stuff from the bluetooth device
    char x = SerialBT.read();
    if(x == 'O'){
      int i = 50;
      while (i > 0)
      {
        on();
        delay(20);
        i--;
      }
      SerialBT.println("Turned On/Off");
    }else if (x != '\n'){
      SerialBT.println("Invalid operation");
    }
  }

  //on();
  //delay(20);//20ms delay before retrying
}

