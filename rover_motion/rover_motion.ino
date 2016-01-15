// defines for bluetooth input
String readString;
String prevCommand = "";

#define FORWARDLEFT   'a'
#define FORWARD       'b'
#define FORWARDRIGHT  'c'
#define LEFT          'd'
#define RIGHT         'e'
#define BACKLEFT      'f'
#define BACK          'g'
#define BACKRIGHT     'h'
#define STOP          's'


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
#define frLeft_R   43
#define frRight_R  51
#define frLeft2_R  49
#define frRight2_R 35
#define left_R     27
#define right_R    31
#define back_R     25

#define frLeft_T   42
#define frRight_T  50
#define frLeft2_T  48
#define frRight2_T 34
#define left_T     26
#define right_T    30
#define back_T     24

long dFL, dFR, dFL2, dFR2, dL, dR, dB;
int safeDist = 6; //inch
int followDist = 20; //inch
int maxDist = 300;
 
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
  Serial.println(readString);

  // if the user gives input, do what the input tells
  if (readString.length() >0) {
    

    if (readString != prevCommand) {
      // call the control functions
      if (readString.charAt(0) == FORWARDLEFT) forward_left();
      else if (readString.charAt(0) == FORWARD) forward();
      else if (readString.charAt(0) == FORWARDRIGHT) forward_right();
      else if (readString.charAt(0) == LEFT) left();
      else if (readString.charAt(0) == RIGHT) right();
      else if (readString.charAt(0) == BACKLEFT) backward_left();
      else if (readString.charAt(0) == BACK) backward();
      else if (readString.charAt(0) == BACKRIGHT) backward_right();
      else if (readString.charAt(0) == STOP) decel();
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
    if (Serial1.available()) return;
    dFR = calcDistance(frRight_T, frRight_R);
    if (Serial1.available()) return;
    dFL2 = calcDistance(frLeft2_T, frLeft2_R);
    if (Serial1.available()) return;
    dFR2 = calcDistance(frRight2_T, frRight2_R);
    if (Serial1.available()) return;
//    dL = calcDistance(left_T, left_R);
//    if (Serial1.available()) return;
//    dR = calcDistance(right_T, right_R);
//    if (Serial1.available()) return;
//    dB = calcDistance(back_T, back_R);
//    if (Serial1.available()) return;

    Serial1.print(dFL);
    Serial1.print(" , ");
    Serial1.print(dFR);
    Serial1.print(" , ");
    Serial1.print(dFL2);
    Serial1.print(" , ");
    Serial1.println(dFR2);

    if (dFL != 0 && dFR != 0 && dFL < maxDist && dFR < maxDist) {
      // if the people is in front of the car
      if (abs(dFL - dFR) <= 4) {
        if (dFL >= followDist) forward();
        else decel();
      }
      else {
        if (dFL < dFR) left();
        else right();
      }
      delay(500);
      return;
    }

    if (dFL == 0 || dFL > maxDist) dFL = maxDist;
    if (dFR == 0 || dFR > maxDist) dFR = maxDist;
    if (dFL2 == 0 || dFL2 > maxDist) dFL2 = maxDist;
    if (dFR2 == 0 || dFR2 > maxDist) dFR2 = maxDist;
    
    int forw = dFL + dFR;
    int lef = dFL + dFL2;
    int rig = dFR + dFR2;

    // if left side has the minimum sum
    if (lef < forw && lef < rig) {
      left();
      delay(500);
      return;
    }
    // if right side has the minimum sum
    else if (rig < forw && rig < lef) {
      right();
      delay(500);
      return;
    }
    else {
      
      delay(500);
    }
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
  long distance = (duration/2) / 74;

  return distance;
}

