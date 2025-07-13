#include <Arduino.h>
#define irLED 16 //irLED on pin 2
#include "BluetoothSerial.h" //for bluetooth connectivity

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;


//vars
unsigned long delayStart = 0;
bool on = false;
bool connectMessage = false;

//array of commands
enum commands {
  POWER = 0b101010000001100,
  //inputs
  SAT = 0b110000000001101,
  SACD = 0b101001000001100,
  BDDVD = 0b01101000000100010100,  //too long
  GAME = 0b00111100001100,
  USB = 0b11101000000100010100, //too long
  TV = 0b010101100001100,

  //commands
  VUP = 0b010010000001100, //volume up
  VDOWN = 0b110010000001100, //volume down
  PAUSE = 0b01011100000100010000, //pause
  NEXT = 0b10001100000100010000, //next
  PREV = 0b00001100000100010000,//prev

};


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

void sendMenu(){
  SerialBT.println();
  SerialBT.println("----Sony Reciever Bluetooth Menu----");
  Serial.print("\033[31m"); // Red text
  SerialBT.println("-- Functions --");
  SerialBT.println("O - Turn receiver on/off");
  SerialBT.println("? - Check receiver power status");
  Serial.print("\033[0m"); // Reset color
  SerialBT.println("M - Request this menu be printed again");
  SerialBT.println("-- Inputs --");
  SerialBT.println("B - BD/DVD    G - Game     U - USB");
  SerialBT.println("S - Sat/Catv  C - SA-CD/CD T - TV");
  SerialBT.println();
}



void send(int command){


  //uint16_t data = 0b101010000001100;
  start(); //send start to IR reciever

  //transmit the menu for easy access


  for(int i = 14; i >= 0; i-- ){
    bool result = command & (1 << i);
    if(result){
      one();
      Serial.println("one");
    }else{
      zero();
      Serial.println("zero");
    }
  }

};


int sendCommand(int command){
  int i = 50;
  while (i > 0)
    {
      send(command);
      delay(20);
      i--;
    }
  return 1;
}

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

  if(SerialBT.connected() && connectMessage == false){
    sendMenu();
    connectMessage = true;
  }

  if(SerialBT.available()){//if we recieve stuff from the bluetooth device
    char x = SerialBT.read();
    if(x == 'O'){
      sendCommand(POWER);
      SerialBT.println("Turned On/Off");
      on ^= 1;//exclusive or to toggle on
    }if(x == 'B'){
      sendCommand(BDDVD);
      SerialBT.println("Input: BD/DVD");
    }if(x == 'G'){
      sendCommand(GAME);
      SerialBT.println("Input: Game");      
    }if(x == 'U'){
      sendCommand(USB);
      SerialBT.println("Input: USB");     
    }if(x == 'S'){
      sendCommand(SAT);
      SerialBT.println("Input: SAT/CATV");     
    }if(x == 'C'){
      sendCommand(SACD);
      SerialBT.println("Input: CD");           
    }if(x == 'T'){
      sendCommand(TV);
      SerialBT.println("Input: TV");     
    }if(x == '?'){
      if(on){
        SerialBT.println("Receiver is ON");
      }else{
        SerialBT.println("Receiver is OFF");
      }
    }if(x == 'M'){
      sendMenu();
    }
    
    
    else if (x != '\n'){
      SerialBT.println("Invalid operation");
    }
  


  
  }

}

