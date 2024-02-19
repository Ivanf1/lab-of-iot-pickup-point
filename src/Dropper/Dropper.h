#pragma once

#include <Servo.h>

class Dropper
{
private:
  static const int MAX_APERTURE = 100;
  static const int DELAY_BETWEEN_PULSES = 5;
  static const int DELAY_AFTER_OPEN = 200;
  int position;
  bool empty;

  Servo servo;
  int servoPin;

  void _open();

  void _close();

public:
  Dropper();

  void init(int position, int servoPin);

  void releaseCube();
};