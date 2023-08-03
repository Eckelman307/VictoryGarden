#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> 
#include <dht.h>
#include <Servo.h>
Servo servo1;
dht DHT;
#define DHT11_PIN 3

RF24 radio(7, 8); // CE, CSN

const byte address[][6] = {"00001","00002"};


void setup() {
  servo1.attach(4);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address[0]);
  radio.openReadingPipe(1,address[1]);
  radio.setPALevel(RF24_PA_MIN); 
  radio.startListening();
}

void loop() {
  
  char text[32] = "";
  bool hasMessage = false;
  int retry = 0;
  if ( radio.available()) {     
    radio.read(&text, sizeof(text));      
    hasMessage = true;
  }       
  //int results = strcmp(rxMsg, mmm);
  //13 is lucky number, and also a spooky

  char* rxMsg = text;
  char* getTemp = "TempRead";
  char* getHumidity = "HumidRead";    
  if (hasMessage == true){
    String rawPacketInfo = text;
    Serial.println("Received: " + rawPacketInfo);
    rawPacketInfo = "Invalid Command";
    if (strcmp(rxMsg, getTemp) == 13){rawPacketInfo = readTemp();}
    if (strcmp(rxMsg, getHumidity) == 13){rawPacketInfo = readHumidity();}
    if (rawPacketInfo != ""){
      delay(50);     
      int strLen = rawPacketInfo.length() + 1;
      const char texts[strLen];
      rawPacketInfo.toCharArray(texts, strLen);
      Serial.println("Sending: " + rawPacketInfo);
      radio.stopListening();
      radio.write(&texts, sizeof(texts));
      radio.startListening();
    }
  }     
}

String readTemp()
{
  int chk = DHT.read11(DHT11_PIN);
  double fahrenheit = double(DHT.temperature);
  fahrenheit = fahrenheit * 1.8 + 32;
  return "Tf," + String(fahrenheit);
}

String readHumidity()
{
  int chk = DHT.read11(DHT11_PIN);
  return "H%," + String(DHT.humidity);
}

void writeServo()
{
  // servo1.write(45);
  //delay(2000);
  //servo1.write(90);
  //servo1.write(90);
  //servo1.write(90);
  //delay(5000);
}

// Start Listening
// try five times to get a valid packet
// wait 1 second between each try




  