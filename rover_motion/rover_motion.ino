/* This program controls 2 separate DC motors using an L298 dual H 
 * bridge motor driver.
 * This program includes the following helper functions:
 * forward, backward, forward_left, forward_right, backward_left
 * backward_right, left, right, accel, decel, brake
 */

#define ENA    10
#define IN1    9
#define IN2    8
#define ENB    5
#define IN3    6
#define IN4    7
#define CRUISE 80
#define ACCEL  10
#define BRAKE  30
#define MAX    100

String message;

void setup() 
{
  Serial1.begin(9600); //set baud rate
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
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
  counterclockwise(IN1, IN2);
  analogWrite(ENB, speed);
  clockwise(IN3, IN4);
}

void right()
{
  int speed = map(CRUISE, 0, 100, 0, 255);
  analogWrite(ENA, speed);
  clockwise(IN1, IN2);
  analogWrite(ENB, speed);
  counterclockwise(IN3, IN4);
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
  /* motion */
//  forward();
//  delay(1000);
//  backward();
//  delay(1000);
//  left();
//  delay(1000);
//  right();
//  delay(1000);
//  forward_left();
//  delay(1000);
  /* bluetooth */
  while(Serial1.available())
  {//while there is data available on the serial monitor
    message += char(Serial1.read());//store string from serial command
  }
  if(!Serial1.available())
  {
    if(message != "")
    {//if data is available
      if (Serial.available())
        Serial.println(message); //show the data
      message = ""; //clear the data
    }
  }
  delay(5000); //delay
}
