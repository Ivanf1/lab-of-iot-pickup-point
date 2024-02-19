#pragma once

#include <Servo.h>

class Dropper
{
private:
  static const int MAX_APERTURE = 100;
  static const int DELAY_BETWEEN_PULSES = 5;
  static const int DELAY_AFTER_OPEN = 200;
  int _position;

  Servo _servo;
  int _servoPin;

  int _irPin;

  void _open();

  void _close();

public:
  Dropper();

  void init(int position, int servoPin, int irPin);

  void releaseCube();

  bool isEmpty();

  bool waitForCubeInsertion();
};