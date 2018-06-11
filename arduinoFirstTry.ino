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

//define the L298N IO pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

int pwm = 127;
char signal;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.begin(9600);
}

void forward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(20);
}

void backward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(20);
}

void leftward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(20);
}

void rightward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
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
}


void setSpeed(int p) {
  pwm = p;
}

void loop() {
  //send data only when receive data
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
    }
  }
}
