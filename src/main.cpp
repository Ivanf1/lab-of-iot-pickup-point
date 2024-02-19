#include <Arduino.h>
#include <PubSubClient.h>

#include <Dropper/Dropper.h>

static const int LEFT_DROPPER_POSITION = 0;
static const int RIGHT_DROPPER_POSITION = 1;

static const int leftServoPin = 19;
static const int leftIrPin = 18;
static const int leftRedLedPin = 21;
static const int leftYellowLedPin = 22;
static const int leftGreenLedPin = 23;

static const int rightServoPin = 26;
static const int rightIrPin = 27;
static const int rightRedLedPin = 25;
static const int rightYellowLedPin = 33;
static const int rightGreenLedPin = 32;

Dropper leftDropper;

void setup() {
  Serial.begin(115200);
  // servo.attach(servoPin);
  // pinMode(irPin, INPUT);
  leftDropper.init(LEFT_DROPPER_POSITION, leftServoPin);

  // pinMode(redLedPin, OUTPUT);
  // pinMode(yellowLedPin, OUTPUT);
  // pinMode(greenLedPin, OUTPUT);
}

void loop() {
  // int state = digitalRead(irPin);

  leftDropper.releaseCube();
  delay(2000);

  // if (state == LOW)
  // {
  //   Serial.println("no");
  // }
  // else
  // {
  //   Serial.println("yes");
  // }

  // delay(10);

  // digitalWrite(redLedPin, LOW);
  // digitalWrite(yellowLedPin, LOW);
  // digitalWrite(greenLedPin, HIGH);

  // delay(2000);

  // digitalWrite(redLedPin, LOW);
  // digitalWrite(yellowLedPin, HIGH);
  // digitalWrite(greenLedPin, LOW);

  // digitalWrite(redLedPin, HIGH);
  // digitalWrite(yellowLedPin, LOW);
  // digitalWrite(greenLedPin, LOW);

  // delay(10000);
}