
String readString;
String prevCommand = "";

#define ENA    10
#define IN1    9
#define IN2    8
#define ENB    5
#define IN3    6
#define IN4    7
#define CRUISE 90
#define ACCEL  10
#define BRAKE  30
#define MAX    100

#define FORWARDLEFT   "a"
#define FORWARD       "b"
#define FORWARDRIGHT  "c"
#define LEFT          "d"
#define RIGHT         "e"
#define BACKLEFT      "f"
#define BACK          "g"
#define BACKRIGHT     "h"

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial1.begin(9600); //set baud rate
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
  int right_speed = map(90, 0, 100, 0, 255);
  analogWrite(ENA, right_speed);
  clockwise(IN1, IN2);
  analogWrite(ENB, left_speed);
  clockwise(IN3, IN4);
}

void forward_right()
{
  int left_speed = map(90, 0, 100, 0, 255);
  int right_speed = map(50, 0, 100, 0, 255);
  analogWrite(ENA, right_speed);
  clockwise(IN1, IN2);
  analogWrite(ENB, left_speed);
  clockwise(IN3, IN4);
}

void backward_left()
{
  int left_speed = map(50, 0, 100, 0, 255);
  int right_speed = map(90, 0, 100, 0, 255);
  analogWrite(ENA, right_speed);
  counterclockwise(IN1, IN2);
  analogWrite(ENB, left_speed);
  counterclockwise(IN3, IN4);
}

void backward_right()
{
  int left_speed = map(90, 0, 100, 0, 255);
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

void loop() {

  //expect a string like wer,qwe rty,123 456,hyre kjhg,
  //or like hello world,who are you?,bye!,
  while (Serial1.available()) {
    char c = Serial1.read();  //gets one byte from serial buffer
    if (c == ' ') {
      break;
    }  //breaks out of captu v     re loop to print readstring
    readString += c; 
  } //makes the string readString  

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
  decel();
  prevCommand = "";
  delay(500);
}


