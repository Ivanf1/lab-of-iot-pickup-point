#pragma once

#include <Servo.h>

class Dropper
{
private:
  enum StatusLED {
    EMPTY,
    FULL,
    AWAITING_INSERTION,
  };

  static const int MAX_APERTURE = 100;
  static const int DELAY_BETWEEN_PULSES = 5;
  static const int DELAY_AFTER_OPEN = 200;
  int _position;
  bool _empty;

  Servo _servo;
  int _servoPin;
  int _irPin;
  int _redLedPin;
  int _yellowLedPin;
  int _greenLedPin;

  void _open();

  void _close();

  bool _isCubeDetected();

  void _onCubeInsertionResult(bool inserted);

  bool _waitForCubeInsertion();

  void _setLed(StatusLED status);

public:
  Dropper();

  void init(int position, int servoPin, int irPin, int redLedPin, int yellowLedPin, int greenLedPin);

  void releaseCube();

  bool isEmpty();

  bool cubeInsertionRequest();
};