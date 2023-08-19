#include "NewPing.h"

#define TRIGGER_PIN_1 9
#define ECHO_PIN_1 10

#define TRIGGER_PIN_2 9
#define ECHO_PIN_2 10

#define TRIGGER_PIN_3 9
#define ECHO_PIN_3 10
// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400

// NewPing setup of pins and maximum distance.
NewPing sonarFront(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonarRight(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonarLeft(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE);

//number of reading to take average distance
int iterations = 5;

float distanceFront, distanceRight, distanceLeft;

// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

void setup() {
  // Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  distanceFront = (sonarFront.ping_median(iterations) / 2) * 0.0343
  distanceRight = (sonarRight.ping_median(iterations) / 2) * 0.0343
  distanceLeft = (sonarLeft.ping_median(iterations) / 2) * 0.0343

  if (distanceFront > 20){
    forward();

  if(distanceLeft > 10 && distanceLeft<15){
      forward();
    }

    if(distanceLeft >=15){
       left();
       delay(30);
       forward();
    }

    if(distanceLeft<10 && distanceLeft>0){
      right();
      delay(30);
      forward();
    }
  }

  if(distanceFront<=10 && distanceRight > 10){
    stop();
    delay(1000);
    right();
    delay(400);
  }

  if(distanceFront<=10 && distanceRight<10){
    stop();
    delay(1000);
    right();
    delay(800);
  }
}

void forward(){
  analogWrite(enA, 255);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	//delay(2000);
}

void right(){
  analogWrite(enA, 255);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
	//delay(2000);
}

void left(){
  analogWrite(enA, 255);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
	//delay(2000);
}

void stop(){
  analogWrite(enA, 255);
	analogWrite(enB, 255);

	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
	//delay(2000); 
}
