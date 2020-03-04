#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define CNS 8
#define CE 7

RF24 radio(CE, CNS);

const byte address[6] = "00001";

int valueTilt = 0;
float valueAngle = 0.00;
bool valueButton = false;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  char text[32] = "";
  char label[8] = "";

  if (radio.available()){
    radio.read(&text, sizeof(text));

    Serial.println(text);
  }
}
