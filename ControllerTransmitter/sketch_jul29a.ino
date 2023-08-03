#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h> 


RF24 radio(7, 8); // CE, CSN

const byte address[][6] = {"00001","00002"};


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address[1]);
  radio.openReadingPipe(1,address[0]);
  radio.setPALevel(RF24_PA_MIN); 
  radio.startListening();
}

void loop() {
  
    char text[32] = "";
    bool hasMessage = false;
    int retry = 0;
    
    if ( radio.available()) {     
      radio.read(&text, sizeof(text));  
      String rxMsg = text;
      Serial.println("Received: " + rxMsg);
    }   

    if (Serial.available()){
      String rawPacketInfo = Serial.readString();
      Serial.println(rawPacketInfo);
      //delay(00);
      int strLen = rawPacketInfo.length() + 1;
      const char texts[strLen];
      rawPacketInfo.toCharArray(texts, strLen);
      radio.stopListening();
      radio.write(&texts, sizeof(texts));
      radio.startListening();     
    }   
}
      




  