// defines for bluetooth input
String readString;
String prevCommand = "";

#define FORWARDLEFT   "a"
#define FORWARD       "b"
#define FORWARDRIGHT  "c"
#define LEFT          "d"
#define RIGHT         "e"
#define BACKLEFT      "f"
#define BACK          "g"
#define BACKRIGHT     "h"


// defines for motor control
#define ENA    10
#define IN1    9
#define IN2    8
#define ENB    5
#define IN3    6
#define IN4    7

#define CRUISE 100
#define ACCEL  10
#define BRAKE  30
#define MAX    100

// Defines for ultrasonic
#define frLeft_R   22
#define frRight_R  24
#define frLeft2_R  26
#define frRight2_R 28
#define left_R     30
#define right_R    32
#define back_R     34

#define frLeft_T   23
#define frRight_T  25
#define frLeft2_T  27
#define frRight2_T 29
#define left_T     31
#define right_T    33
#define back_T     35

long dFL, dFR, dFL2, dFR2, dL, dR, dB;
int safeDist = 15; //cm
int followDist = 300; //cm 
 
void setup() {
  //Serial Port begin
  Serial.begin(9600);
  // serial for bluetooth
  Serial1.begin(9600);

  // Define pins for motor
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  //Define inputs and outputs pins for ultrasonic
  pinMode(frLeft_T, OUTPUT);
  pinMode(frRight_T, OUTPUT);
  pinMode(frLeft2_T, OUTPUT);
  pinMode(frRight2_T, OUTPUT);
  pinMode(left_T, OUTPUT);
  pinMode(right_T, OUTPUT);
  pinMode(back_T, OUTPUT);

  pinMode(frLeft_R, INPUT);
  pinMode(frRight_R, INPUT);
  pinMode(frLeft2_R, INPUT);
  pinMode(frRight2_R, INPUT);
  pinMode(left_R, INPUT);
  pinMode(right_R, INPUT);
  pinMode(back_R, INPUT);
}

void clockwise(int in1, int in2)
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void counterclockwise(int in1, int in2)
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void forward()
{
  int speed = map(CRUISE, 0, 100, 0, 255);
  analogWrite(ENA, speed);
  clockwise(IN1, IN2);
  analogWrite(ENB, speed);
  clockwise(IN3, IN4);
}

void backward()
{
  int speed = map(CRUISE, 0, 100, 0, 255);
  analogWrite(ENA, speed);
  counterclockwise(IN1, IN2);
  
  analogWrite(ENB, speed);
  counterclockwise(IN3, IN4);
}

void forward_left()
{
  int left_speed = map(50, 0, 100, 0, 255);
  int right_speed = map(100, 0, 100, 0, 255);
  analogWrite(ENA, right_speed);
  clockwise(IN1, IN2);
  analogWrite(ENB, left_speed);
  clockwise(IN3, IN4);
}

void forward_right()
{
  int left_speed = map(100, 0, 100, 0, 255);
  int right_speed = map(50, 0, 100, 0, 255);
  analogWrite(ENA, right_speed);
  clockwise(IN1, IN2);
  analogWrite(ENB, left_speed);
  clockwise(IN3, IN4);
}

void backward_left()
{
  int left_speed = map(50, 0, 100, 0, 255);
  int right_speed = map(100, 0, 100, 0, 255);
  analogWrite(ENA, right_speed);
  counterclockwise(IN1, IN2);
  analogWrite(ENB, left_speed);
  counterclockwise(IN3, IN4);
}

void backward_right()
{
  int left_speed = map(100, 0, 100, 0, 255);
  int right_speed = map(50, 0, 100, 0, 255);
  analogWrite(ENA, right_speed);
  counterclockwise(IN1, IN2);
  analogWrite(ENB, left_speed);
  counterclockwise(IN3, IN4);
}

void left()
{
  int speed = map(CRUISE, 0, 100, 0, 255);
  analogWrite(ENA, speed);
  clockwise(IN1, IN2);
  analogWrite(ENB, speed);
  counterclockwise(IN3, IN4);
}

void right()
{
  int speed = map(CRUISE, 0, 100, 0, 255);
  analogWrite(ENA, speed);
  counterclockwise(IN1, IN2);
  analogWrite(ENB, speed);
  clockwise(IN3, IN4);
}

void decel()
{
  analogWrite(ENA, LOW);
  analogWrite(ENB, LOW);
}

void brake()
{
  analogWrite(ENA, BRAKE);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, BRAKE);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
 
void loop()
{
  //expect a string
  while (Serial1.available()) {
    char c = Serial1.read();  //gets one byte from serial buffer
    if (c == ' ') {
      // break out of the loop seeing " "
      break;
    }
    //makes the string readString  
    readString += c;
  }

  // if the user gives input, do what the input tells
  if (readString.length() >0) {

    if (readString != prevCommand) {
      // call the control functions
      if (readString == FORWARDLEFT) forward_left();
      else if (readString == FORWARD) forward();
      else if (readString == FORWARDRIGHT) forward_right();
      else if (readString == LEFT) left();
      else if (readString == RIGHT) right();
      else if (readString == BACKLEFT) backward_left();
      else if (readString == BACK) backward();
      else if (readString == BACKRIGHT) backward_right();
      prevCommand = readString;
    }

    readString=""; //clears variable for new input
    delay(500);
    return;
  }
  else {
    
    // calculate how far the nearest object is in front of 
    // each ultrasonic sensor
    dFL = calcDistance(frLeft_T, frLeft_R);
    dFR = calcDistance(frRight_T, frRight_R);
    dFL2 = calcDistance(frLeft2_T, frLeft2_R);
    dFR2 = calcDistance(frRight2_T, frRight2_R);
    dL = calcDistance(left_T, left_R);
    dR = calcDistance(right_T, right_R);
    dB = calcDistance(back_T, back_R);
  
    // if the people is in front of the car
    if (dFL == dFR) {
      if (dFL >= followDist) forward();
      else decel();
    }
    else {
      if (dFL < followDist && dFR < followDist) {
        if (dFL < dFR) left();
        else right();
      }
      else {
        if (dFL < dFR) forward_left();
        else forward_right();
      }
    }
    
    delay(500);
  }
}

long calcDistance(int trigPin,int echoPin)
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration/2) / 29.1;

  return distance;
}

