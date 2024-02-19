#include <Arduino.h>
#include <Servo.h>

static const int servoPin = 13;
static const int irPin = 14;

static const int redLedPin = 25;
static const int yellowLedPin = 33;
static const int greenLedPin = 32;

Servo servo;

void drop_cube(void) {
  for (int posDegrees = 0; posDegrees <= 100; posDegrees++) {
    servo.write(posDegrees);
    delay(5);
  }

  delay(100);

  for (int posDegrees = 100; posDegrees >= 0; posDegrees--) {
    servo.write(posDegrees);
    delay(5);
  }
}

void on_cube_drop_message_received(void) {}

void setup() {
  Serial.begin(115200);
  servo.attach(servoPin);
  pinMode(irPin, INPUT);

  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  int state = digitalRead(irPin);

  // if (state == LOW)
  // {
  //   Serial.println("no");
  // }
  // else
  // {
  //   Serial.println("yes");
  // }

  // delay(10);

  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);

  delay(2000);

  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, HIGH);
  digitalWrite(greenLedPin, LOW);

  digitalWrite(redLedPin, HIGH);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(greenLedPin, LOW);

  delay(10000);
}