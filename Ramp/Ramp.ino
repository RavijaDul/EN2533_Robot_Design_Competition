#include <MPU6050.h>
#include <math.h>
#include <Wire.h>
#include <Servo.h>

#define IR0 52
#define IR1 A0//11
#define IR2 A1//12
#define IR3 A2//13
#define IR4 A3//2
#define IR5 A4//3
#define IR6 A5//4
#define IR7 53

#define ENA 2//5  //RIGHT
#define motorInput1 3//7 
#define motorInput2 4//8
#define motorInput3 6//9
#define motorInput4 5//10
#define ENB 7//6

#define MAX_SPEED 255

int IR_val[] = {0, 0, 0, 0, 0, 0, 0, 0};
int IR_weights[8] = {0, -15, -10, -5, 5, 10, 15, 0};

int RMotor = 0;
int LMotor = 0;
int speedAdjust = 0;
int baseSpeed = 85; //final = 75

float P, I, D;
float error = 0;
float previousError = 0;
float Kp = 12;//11 ok 12th ok 10 not ok
float Kd = 0.1;//10, 1 final = 6 //7,8,9 THIBBE
float Ki = 0; //final = 0

MPU6050 mpu;
const float gravity = 9.81; // acceleration due to gravity in m/s^2
const float wheel_radius = 32.5/1000; // radius of the wheel in meters
const float robot_mass = 1.0; // mass of the robot in kilograms
const float max_speed = 5.24;//max speed of robot in ms^-1 //0.3

Servo myservoUp;
Servo myservoDown;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  mpu.initialize();

  myservoUp.attach(10);  // attaches the servo on pin 9 to the servo object
  myservoDown.attach(9); 
  myservoUp.write(0);
  delay(500);
  myservoDown.write(0);
  delay(500);
  myservoUp.write(170);
  delay(100);
  myservoDown.write(70);
  delay(100);
  
  Serial.begin(9600);
  pinMode(IR0, INPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);
  pinMode(IR6, INPUT);
  pinMode(IR7, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(motorInput1, OUTPUT);
  pinMode(motorInput2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(motorInput3, OUTPUT);
  pinMode(motorInput4, OUTPUT);

  // analogWrite(ENA, 0);
  // analogWrite(ENB, 255);
  set_forward();
  delay(200);

}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  float accelX = ax / 16384.0*9.81;
  float accelY = ay / 16384.0*9.81;
  float accelZ = az / 16384.0*9.81;

  double totalAccel = sqrt(pow(accelX,2)+pow(accelY,2) + pow(accelZ,2));

  double tiltAngleRad = acos(accelZ / totalAccel);

// Convert tilt angle to degrees
  double tiltAngleDeg = tiltAngleRad * (180.0 / PI);

  if (tiltAngleDeg>15){
    baseSpeed = 175;

  }
  if (tiltAngleDeg<0){
    baseSpeed = 70;
  }
  lineFollow();
}

void lineFollow(){
  //read_IR();
  // if (IR_val[0]==0 && IR_val[1]==0 && IR_val[2]==0 && IR_val[3]==0 && IR_val[4]==0 && IR_val[5]==0 && IR_val[6]==0 && IR_val[7]==0){
  //   //analogWrite(ENB, 50);
  //   //delay(100);
  //   stop();
  //   while (1){

  //   }
  // }

  analogWrite(ENA, 200);
  analogWrite(ENB, 200);

  set_forward();
  //PID_control();
  //set_speed();
}

int minSpeed =10;

void PID_control() {
  error = 0;

  for (int i = 0; i < 6; i++)
  {
    error += IR_weights[i+1]*IR_val[i+1];
  }
  P = error;
  I = I + error; 
  D = error - previousError;

  previousError = error;

  speedAdjust = (Kp*P + Ki*I + Kd*D);;

  RMotor = baseSpeed + speedAdjust;
  LMotor = baseSpeed - speedAdjust;

  if (IR_val[1]==1 && IR_val[6]==1){
    if (RMotor<minSpeed){
      RMotor = minSpeed;
  }
    if (LMotor<minSpeed){
        LMotor = minSpeed;
    }
  }
  else {
  if (RMotor<0){
    analogWrite(ENA, 50);
    analogWrite(ENB, LMotor);

    digitalWrite(motorInput1, HIGH);
    digitalWrite(motorInput2, LOW);
    digitalWrite(motorInput3, HIGH);
    digitalWrite(motorInput4, LOW);
  }
  if (LMotor<0){
    analogWrite(ENA, RMotor);
    analogWrite(ENB, 50);

    digitalWrite(motorInput1, LOW);
    digitalWrite(motorInput2, HIGH);
    digitalWrite(motorInput3, LOW);
    digitalWrite(motorInput4, HIGH);
  }
  }
  if (LMotor>MAX_SPEED){
    LMotor = MAX_SPEED;
  }
  if (RMotor>MAX_SPEED){
    RMotor = MAX_SPEED;
  }
  Serial.print(LMotor);
  Serial.print(" ");
  Serial.println(RMotor);

}

void read_IR(){
  IR_val[0] = digitalRead(IR0);
  IR_val[1] = digitalRead(IR1);
  IR_val[2] = digitalRead(IR2);
  IR_val[3] = digitalRead(IR3);
  IR_val[4] = digitalRead(IR4);
  IR_val[5] = digitalRead(IR5);
  IR_val[6] = digitalRead(IR6);
  IR_val[7] = digitalRead(IR7);
  
  Serial.print(IR_val[1]);
  Serial.print(IR_val[2]);
  Serial.print(IR_val[3]);
  Serial.print(IR_val[4]);
  Serial.print(IR_val[5]);
  Serial.println(IR_val[6]);
}

void set_speed(){
  analogWrite(ENA, RMotor);
  analogWrite(ENB, LMotor);
}

void set_forward(){
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, HIGH);
  digitalWrite(motorInput3, HIGH);
  digitalWrite(motorInput4, LOW);
}

void stop(){
  digitalWrite(motorInput1, LOW);
  digitalWrite(motorInput2, LOW);
  digitalWrite(motorInput3, LOW);
  digitalWrite(motorInput4, LOW);
}