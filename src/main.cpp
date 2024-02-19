#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "wifikeys.h"
#include <Dropper/Dropper.h>

#include <esp_log.h>

#define TAG "MAIN"

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

static const int WAIT_TIME_BEFORE_RECONNECT = 1000;

Dropper leftDropper;

WiFiClient client;
PubSubClient mqttClient(client);

void on_mqtt_message_received(char* topic, byte* payload, unsigned int length) {
  ESP_LOGD(TAG, "Message arrived in topic: %s", topic);
  // for (int i = 0; i < length; i++) {
  //   ESP_LOGD(TAG, "%c", (char)payload[i]);
  // }
  // ESP_LOGD(TAG, "\n");
}

void mqtt_connect() {
  while (!mqttClient.connected()) {
    ESP_LOGD(TAG, "Attempting MQTT connection");
    if (mqttClient.connect("pickup-point-1")) {
      ESP_LOGD(TAG, "MQTT connection established");
      mqttClient.subscribe("sm_iot_lab/pickup");
    } else {
      ESP_LOGD(TAG, "MQTT connection failed. Try again in %d seconds", WAIT_TIME_BEFORE_RECONNECT);
      delay(WAIT_TIME_BEFORE_RECONNECT);
    }
  }
}

void setup() {
  Serial.begin(115200);
  // servo.attach(servoPin);
  // pinMode(irPin, INPUT);
  leftDropper.init(LEFT_DROPPER_POSITION, leftServoPin);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  mqttClient.setServer(BROKER_IP, BROKER_PORT);
  mqttClient.setCallback(on_mqtt_message_received);

  // pinMode(redLedPin, OUTPUT);
  // pinMode(yellowLedPin, OUTPUT);
  // pinMode(greenLedPin, OUTPUT);
  mqtt_connect();
}

void loop() {
  // int state = digitalRead(irPin);

  // leftDropper.releaseCube();
  // delay(2000);

  if (!mqttClient.connected()) {
    ESP_LOGD(TAG, "mqtt client disconnected");
    mqtt_connect();
  }
  mqttClient.loop();

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