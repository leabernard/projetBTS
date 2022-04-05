#include <SPI.h>
#include <Ethernet.h>

//Pin names
#define LED1 2 //D2
#define LED2 3  //D3
#define LED3 4  //D4
#define BUTTON1 6 //D6
#define BUTTON2 7 //D7
#define BUTTON3 8 //D8
#define WEIGHT1 A1//A1
#define WEIGHT2 A2 //A2
#define WEIGHT3 A3 //A3
//Ethernet parameters
byte MAC[] = {0x2C, 0xF7, 0xF1, 0x08, 0x3C, 0x2A};
byte ip[] = {192, 168, 64, 201};
byte server[] = {192, 168, 64, 232};
int retry = 0;

EthernetClient client;

void setup() {
  //Initialize ethernet
  Ethernet.begin(MAC, ip);
  Serial.begin(9600);
  delay(1000);
  //Initialize pin
  pinMode(LED1, INPUT);
  pinMode(LED2, INPUT);
  pinMode(LED3, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(WEIGHT1, OUTPUT);
  pinMode(WEIGHT2, OUTPUT);
  pinMode(WEIGHT3, OUTPUT);

  //connect to server
  connection();
  
}

void loop() {
  delay(150);
  //check if connected. If not try a new connection
  if(client.connected() != true)
  {
    connection();
  }
  char message[200];
  
  bool button1 = digitalRead(BUTTON1);
  bool button2 = digitalRead(BUTTON2);
  bool button3 = digitalRead(BUTTON3);
  
  if(button1)
  {
    digitalWrite(LED1, HIGH);
  }else{
    digitalWrite(LED1, LOW);
  }

  if(button2)
  {
    digitalWrite(LED1, HIGH);
  }else{
    digitalWrite(LED1, LOW);
  }

  if(button3)
  {
    digitalWrite(LED1, HIGH);
  }else{
    digitalWrite(LED1, LOW);
  }
   
  
  float weight3 = (float)analogRead(WEIGHT3);
  float weight2 = (float)analogRead(WEIGHT2);
  float weight1 = (float)analogRead(WEIGHT1);
  Serial.println(weight1);
  char W1[10];
  char W2[10];
  char W3[10];
  dtostrf(weight1, 4, 2, W1);
  dtostrf(weight2, 4, 2, W2);
  dtostrf(weight3, 4, 2, W3);


  snprintf(message, 200, "%c;%c;%c;%s;%s;%s\n", button1 ? '1' : '0', button2 ? '1' : '0', button3 ? '1' : '0', W1, W2, W3);
  //Serial.println(response);
  client.write(message, strlen(message));
  
  
}
//Method to try 3 connections in a row 
void connection() {
  
  while(!(client.connect(server, 502)) && retry < 3)
  {
    retry++;
    Serial.println("autre");
    
  }
  if(retry < 3)
    Serial.println("connecté");
  retry = 0;

}
