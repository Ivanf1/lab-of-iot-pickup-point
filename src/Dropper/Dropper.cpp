#include <Dropper/Dropper.h>

#include <Arduino.h>

Dropper::Dropper() {}

void Dropper::init(int position, int servoPin) {
  this->_position = position;
  this->_servoPin = servoPin;
  this->_empty = true;

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

  this->_empty = true;
}

bool Dropper::isEmpty() { return _empty; }

void Dropper::onCubeInserted() { this->_empty = false; }
