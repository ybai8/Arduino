
/*
 * Author : Yang Bai
 * L298N IO pin         Arduino UNO IO pin
 * ENA                  5
 * ENB                  6
 * IN1                  7
 * IN2                  8
 * IN3                  9
 * IN4                  11
 * 
 * Left motor truth table
 * ENA      IN1        IN2         Description
 * High     Low        Low         Stopped(breaks)
 * High     High       Low         Forwards
 * High     Low        High        Backwards
 * High     High       High        Stopped
 * 
 * Right motor truth table
 * ENB      IN3        IN4         Description
 * High     Low        Low         Stopped(breaks)
 * High     Low        High        Forwards
 * High     High       Low         Backwards
 * High     High       High        Stopped
 */
#include <SoftwareSerial.h>
#include <Servo.h>

//define the L298N IO pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

//define line tracking IO pin
#define LT_R 10
#define LT_M 4
#define LT_L 2

int pwm = 191;
char signal;
Servo myservo;
int Echo = A4;
int Trig = A5;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LT_R, INPUT);
  pinMode(LT_M, INPUT);
  pinMode(LT_L, INPUT);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  myservo.attach(3);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  Serial.begin(9600);
}

void forward() {
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(20);
}

void backward() {
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(20);
}

void leftward() {
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(200);
}

void rightward() {
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(20);
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


void setSpeed(int p) {
  pwm = p;
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
}

void lineTracking() {
  while(!digitalRead(LT_M) || !digitalRead(LT_R) || !digitalRead(LT_L)) {
    while(!digitalRead(LT_M)) {
      forward();
    }
    while(!digitalRead(LT_R)) {
      rightward();
    }
    while(!digitalRead(LT_L)) {
      leftward();
    }
  }
  stop();
}

int getDistance() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float distance = pulseIn(Echo, HIGH);
  distance /= 58;
  return (int) distance;
}

void turn() {
  stop();
  myservo.write(155);
  delay(500);
  leftDistance = getDistance();
  if(leftDistance > 30) {
    leftward();
    delay(200);
  } else {
    myservo.write(0);
    delay(1000);
    rightDistance = getDistance();
    if(rightDistance > 30) {
      rightward();
      delay(200);
    } else {
      backward();
      delay(200);
      int x = random(1);
      if(x == 0) {
        leftward();
        delay(200);
      } else {
        rightward();
        delay(200);
      }
    }
  }
}

void obstacleAvoidance() {
  while(!Serial.available()) {
    //optimize for table leg
    myservo.write(60);
    delay(500);
    middleDistance = getDistance();
    for(int angle = 75; angle <= 90; angle += 15) {
      myservo.write(angle);
      delay(500);
      middleDistance = min(middleDistance, getDistance());
    }
    if(middleDistance < 20) {
      backward();
      delay(300);
    } else if(middleDistance <= 40 && middleDistance > 20) {
      turn();
    } else {
      forward();
    }
  }
  stop();
}

void loop() {
  if(Serial.available() > 0) { 
    signal = Serial.read();
    switch (signal) {
      case 'a' : stop(); break;
      case 'b' : forward(); break;
      case 'c' : backward(); break;
      case 'd' : leftward(); break;
      case 'e' : rightward(); break;
      case 'f' : setSpeed(127); break; // 50% duty cycle
      case 'g' : setSpeed(191); break; // 75% duty cycle
      case 'h' : setSpeed(255); break; // 100% duty cycle
      case 'i' : lineTracking(); break;
      case 'j' : obstacleAvoidance(); break;
    }
  }
}
