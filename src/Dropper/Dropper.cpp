#include <Dropper/Dropper.h>

#include <Arduino.h>

#define TAG "DROPPER"

Dropper::Dropper() {}

void Dropper::init(int position, int servoPin, int irPin, int redLedPin, int yellowLedPin, int greenLedPin) {
  this->_position = position;
  this->_servoPin = servoPin;
  this->_irPin = irPin;
  this->_redLedPin = redLedPin;
  this->_yellowLedPin = yellowLedPin;
  this->_greenLedPin = greenLedPin;

  this->_servo.attach(this->_servoPin);
}

void Dropper::_open() {
  for (int posDegrees = 0; posDegrees <= MAX_APERTURE; posDegrees++) {
    this->_servo.write(posDegrees);
    delay(DELAY_BETWEEN_PULSES);
  }
}

void Dropper::_close() {
  for (int posDegrees = MAX_APERTURE; posDegrees >= 0; posDegrees--) {
    this->_servo.write(posDegrees);
    delay(DELAY_BETWEEN_PULSES);
  }
}

void Dropper::releaseCube() {
  _open();
  delay(DELAY_AFTER_OPEN);
  _close();

  digitalWrite(this->_redLedPin, LOW);
  digitalWrite(this->_greenLedPin, LOW);
  digitalWrite(this->_yellowLedPin, HIGH);
}

bool Dropper::isEmpty() { return digitalRead(this->_irPin) == HIGH; }

void Dropper::_onCubeInsertionResult(bool inserted) {
  if (inserted) {
    digitalWrite(this->_redLedPin, HIGH);
    digitalWrite(this->_greenLedPin, LOW);
    digitalWrite(this->_yellowLedPin, LOW);
  } else {
    digitalWrite(this->_redLedPin, LOW);
    digitalWrite(this->_greenLedPin, LOW);
    digitalWrite(this->_yellowLedPin, HIGH);
  }
}

bool Dropper::onCubeInsertionRequest() {
  digitalWrite(this->_redLedPin, LOW);
  digitalWrite(this->_greenLedPin, HIGH);
  digitalWrite(this->_yellowLedPin, LOW);

  bool inserted = this->_waitForCubeInsertion();

  this->_onCubeInsertionResult(inserted);

  return inserted;
}

bool Dropper::_waitForCubeInsertion() {
  ESP_LOGD(TAG, "Waiting for cube to be inserted");
  int i = 0;
  while (digitalRead(this->_irPin) == HIGH && i <= 50) {
    i++;
    ESP_LOGD(TAG, ".");
    delay(150);
  }

  return i <= 50;
}