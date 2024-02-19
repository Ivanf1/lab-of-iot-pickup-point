#include <Dropper/Dropper.h>

#include <Arduino.h>

Dropper::Dropper() {}

void Dropper::init(int position, int servoPin) {
  this->position = position;
  this->servoPin = servoPin;

  this->servo.attach(this->servoPin);
}

void Dropper::_open() {
  for (int posDegrees = 0; posDegrees <= MAX_APERTURE; posDegrees++) {
    servo.write(posDegrees);
    delay(DELAY_BETWEEN_PULSES);
  }
}

void Dropper::_close() {
  for (int posDegrees = MAX_APERTURE; posDegrees >= 0; posDegrees--) {
    servo.write(posDegrees);
    delay(DELAY_BETWEEN_PULSES);
  }
}

void Dropper::releaseCube() {
  _open();
  delay(DELAY_AFTER_OPEN);
  _close();
}
