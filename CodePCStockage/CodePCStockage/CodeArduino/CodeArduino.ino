#include <SPI.h>
#include <Ethernet.h>
#include <IniFile.h>
#include <SD.h>
#include <Ultrasonic.h>

//Pin names
#define LED1 2 //D2
#define LED2 3  //D3
#define LED3 5  //D5
#define BUTTON1 6 //D6
#define BUTTON2 7 //D7
#define BUTTON3 8 //D8
//const int chipSelect = 4;
//Ethernet parameters
byte MAC[] = {0x2C, 0xF7, 0xF1, 0x08, 0x3C, 0x2A};
Ultrasonic ultrasonic(4);
const size_t bufferLen = 80;
char server[bufferLen];
//byte server[] = {192, 168, 65, 104};
//true when elevator is up 
bool elevator1 = false;
bool elevator2 = false;
bool elevator3 = false;


EthernetClient client;

void setup() {
  //Initialize ethernet
  Ethernet.begin(MAC);
  Serial.begin(9600);
  SPI.begin();
  delay(1000);
  //Initialize pin
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);

  pinMode(4, INPUT);
  //connect to server
  //connection();
  
}

void loop() {
  delay(150);
  //check if connected. If not try a new connection
  if(client.connected() != true)
  {
    connection();
  }else{
    char message[200];
  
    if(client.available()){
      char c = client.read();
      if(c == '1'){
        elevator1 = true;
        digitalWrite(LED1, HIGH);
      }else if(c == '2'){
        elevator2 = true;
        digitalWrite(LED2, HIGH);
      }else if(c == '3'){
        elevator3 = true;
        digitalWrite(LED3, HIGH);
      }
      Serial.println(c);
    }
  
    bool button1 = digitalRead(BUTTON1);
    bool button2 = digitalRead(BUTTON2);
    bool button3 = digitalRead(BUTTON3);

    long rangeInCentimeters = ultrasonic.MeasureInCentimeters();
    Serial.println(rangeInCentimeters);
    
    if(button1)
    {
      elevator1 = false;
      digitalWrite(LED1, LOW);
    }
  
    if(button2)
    {
      elevator2 = false;
      digitalWrite(LED2, LOW);
    }
  
    if(button3)
    {
      elevator3 = false;
      digitalWrite(LED3, LOW);
    }
  
    char measure[10];
    dtostrf(rangeInCentimeters, 3, 0, measure);
    snprintf(message, 200, "%c;%c;%c;%c;%c;%c;%s\n", button1 ? '1' : '0', button2 ? '1' : '0', button3 ? '1' : '0', elevator1 ? '1' : '0', elevator2 ? '1' : '0', elevator3 ? '1' : '0', measure);
    client.write(message, strlen(message));

  }
  
}
//Method to try 3 connections in a row 
void connection() {
  while(!(client.connect("192.168.65.104", 502)))
  {
    Serial.println("autre");
    
  }
  
    Serial.println("connecté");

}
