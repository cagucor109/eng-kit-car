#include <nRF24L01.h>
#include <SPI.h>
#include <RF24.h>
#include <math.h>

#define joy_button 2
#define joy_X A1
#define joy_Y A0

int xPos = 0;
int yPos = 0;
int buttonState = 0;
int mapX = 0;
int mapY = 0; 
double angle = 0;
int tilt = 0;

#define CSN 7
#define CE 8

RF24 radio(CE, CSN);

const byte address[6] = "00001";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  pinMode(joy_X, INPUT);
  pinMode(joy_Y, INPUT);
  pinMode(joy_button, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  char textAngle[16] = "";
  char textTilt[16] = "";
  char textButton[16] = "";
  char angleBuffer[16] = "";
  char text[32] = "";

  xPos = analogRead(joy_X);
  delay(20);
  yPos = analogRead(joy_Y);
  delay(20);
  buttonState = digitalRead(joy_button);
  mapX = map(xPos, 0, 1023, -512, 512);
  mapY = map(yPos, 0, 1023, 512, -512);

  
  // find the angle
  if(abs(mapX) > 80 || abs(mapY) > 80){
    angle = atan(abs(mapX)/abs(mapY));
    // angles defined -Pi -> Pi. 
    // 0 radians is forward, positive radians is left turn, negative radians is right turn
    if(mapX > 0 && mapY > 0){
      angle = modifiedMap(angle, 0, 1.57, 0, -1.57);
    }else if(mapX > 0 && mapY < 0){
      angle = modifiedMap(angle, 0, 1.57, -3.14, -1.57);
    }else if(mapX < 0 && mapY < 0){
      angle = modifiedMap(angle, 0, 1.57, 3.14, 1.57);
    }else{
      angle = modifiedMap(angle, 0, 1.57, 0, 1.57); // left here in case conversion to degrees wanted
    }
    dtostrf(angle, 4, 2, angleBuffer);
    sprintf(textAngle, "angle %s", angleBuffer); 
  }

  // find tilt
  if(abs(mapX) > 450 || abs(mapY) > 450){
    tilt = 2;
  }else if(abs(mapX) > 80 || abs(mapY) > 80){
    tilt = 1;
  }else{
    tilt = 0;
  }
  
  sprintf(textTilt, "tilt %i", tilt); 

  sprintf(textButton, "button %s", buttonState ? "false" : "true"); 

  sprintf(text, "%s\n%s\n%s", textTilt, textAngle, textButton);

  Serial.println(text);
  
  radio.write(&text, sizeof(text));
  
  delay(1000);
}

double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
