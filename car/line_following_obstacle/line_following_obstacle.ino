#include <NewPing.h>

//Arduino Line Follower Obstacle Avoidance Robot

/*You have to install the NewPing library before uploading the code
  Follow this procedure to install the NewPing library First download the NewPing.zip library >> click on sketch tab >> Include Library
  >> Add.ZIP LIBRARY >> Select NewPing.zip file >> Done
  
  Created By DIY Builder
*/

// NewPing Library Link NewPing library Link https://github.com/eliteio/Arduino_New_Ping //
// You can Modify this code according to your need//

//PWM pins (Digital) = 3, 5, 6, 9, 10, 11
int ENA = 3;  //ENA connected to digital pin 3
int ENB = 10;  //ENB connected to digital pin 9
int MOTOR_A1 = 2; // MOTOR_A1 connected to digital pin 4
int MOTOR_A2 = 4; // MOTOR_A2 connected to digital pin 5
int MOTOR_B1 = 5; // MOTOR_B1 connected to digital pin 6
int MOTOR_B2 = 6; // MOTOR_B2 connected to digital pin 7

int IR_RIGHT = A1; // RIGHT sensor connected to analog pin A1
int IR_LEFT = A0;  // LEFT sensor connected to analog pin A0

#define TRIG_FRONT A2 // TRIG PIN connected to analog pin A2
#define ECHO_FRONT A3 // ECHO PIN connected to analog pin A3

#define TRIG_RIGHT A7 // TRIG PIN connected to analog pin A7
#define ECHO_RIGHT A6 // ECHO PIN connected to analog pin A6

#define TRIG_LEFT A5 // TRIG PIN connected to analog pin A5
#define ECHO_LEFT A4 // ECHO PIN connected to analog pin A4

#define MAX_DISTANCE 100 // Define Maximum Distance

NewPing sonar_front(TRIG_FRONT, ECHO_FRONT, MAX_DISTANCE); 
NewPing sonar_right(TRIG_RIGHT, ECHO_RIGHT, MAX_DISTANCE); 
NewPing sonar_left(TRIG_LEFT, ECHO_LEFT, MAX_DISTANCE); 

int distance_front;
int distance_right;
int distance_left;
bool backing, avoid_right, avoid_left, avoid_forward;

void setup() {

  pinMode(ENA, OUTPUT); // initialize ENA pin as an output
  pinMode(ENB, OUTPUT); // initialize ENB pin as an output
  pinMode(MOTOR_A1, OUTPUT); // initialize MOTOR_A1 pin as an output
  pinMode(MOTOR_A2, OUTPUT); // initialize MOTOR_A2 pin as an output
  pinMode(MOTOR_B1, OUTPUT); // initialize MOTOR_B1 pin as an output
  pinMode(MOTOR_B2, OUTPUT); // initialize MOTOR_B2 pin as an output
  pinMode(IR_RIGHT, INPUT); // initialize RIGHT pin as an input
  pinMode(IR_LEFT, INPUT);  // initialize LEFT pin as an input
  
  Serial.begin(9600);

  backing = false;
}

void loop() {
  distance_front = sonar_front.ping_cm();
  if (distance_front == 0) {
    distance_front = 40;
  }
  distance_right = sonar_right.ping_cm();
  if (distance_right == 0) {
    distance_right = 40;
  }
  distance_left = sonar_left.ping_cm();
  if (distance_left == 0) {
    distance_left = 40;
  }
  
  line_follow("black");


//// this is the case if no obstacle avoidance is happening
//  if(!backing && !avoid_right && !avoid_left && !avoid_forward){
//   if(distance_front > 7){
//    line_follow("black");
//    backing = false;
//   }else{
//    move("stop", 0);
//    backing = true;
//   } 
//  }
//
//// when an obstacle is detected, first reverse to gain more space
//  if(backing){
//    if(distance_front < 12){
//      move("backwards", 40);
//    }else{
//      if(distance_right > 10){
//        avoid_right = true;
//        backing = false;
//      }else if(distance_left > 10){
//        avoid_left = true;
//        backing = false;
//      }else{
//        move("backwards", 40);
//      }
//    }
//  }
//
//  if(avoid_right){
//    move("right", 40);
//  }
//
//  if(avoid_left){
//    move("left", 40);
//  }
}

// follow a white line or black line

void line_follow (String colour){
  int leftVal = digitalRead(IR_LEFT);
  int rightVal = digitalRead(IR_RIGHT);

  Serial.print("Right:   ");
  Serial.println(rightVal);
  Serial.print("Left:   ");
  Serial.println(leftVal);
  
  if(colour == "black"){
    if(rightVal && !leftVal){
      move("right", 35);
    }else if(leftVal && !rightVal){
      move("left", 35);
    }else{
      move("forward", 35);
    }
  }else if(colour == "white"){
    if(!rightVal){
      move("right", 35);
    }else if(leftVal){
      move("left", 35);
    }else{
      move("forward", 40);
    }
  }
  delay(50);
}

// specify direction and speed in percentage

void move (String direction, int speed){
  int power = speed * 255 / 100;
  
  if(direction == "forward"){
    analogWrite(ENA, power);
    analogWrite(ENB, power);
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);   
    delay(100);
  }else if(direction == "right"){
    analogWrite(ENA, power);
    analogWrite(ENB, power);
    digitalWrite(MOTOR_A1, HIGH);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);  
    delay(100); 
  }else if(direction == "left"){
    analogWrite(ENA, power);
    analogWrite(ENB, power);
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, HIGH);
    digitalWrite(MOTOR_B1, HIGH);
    digitalWrite(MOTOR_B2, LOW);   
    delay(100);
  }else if(direction == "backwards"){
    analogWrite(ENA, power);
    analogWrite(ENB, power);
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, HIGH);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, HIGH);   
    delay(100);
  }else{
    analogWrite(ENA, power);
    analogWrite(ENB, power);
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, LOW);  
    delay(100); 
  }
}
