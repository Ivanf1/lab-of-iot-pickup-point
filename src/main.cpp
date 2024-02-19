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

static const char* release_cube_0_topic = "sm_iot_lab/pickup_point/0/cube/0/release";
static const char* insert_request_cube_0_topic = "sm_iot_lab/pickup_point/0/cube/0/insert_request";
static const char* insert_response_cube_0_topic = "sm_iot_lab/pickup_point/0/cube/0/inserted";

static const char* release_cube_1_topic = "sm_iot_lab/pickup_point/0/cube/1/release";
static const char* insert_request_cube_1_topic = "sm_iot_lab/pickup_point/0/cube/1/insert_request";
static const char* insert_response_cube_1_topic = "sm_iot_lab/pickup_point/0/cube/1/inserted";

Dropper leftDropper;
Dropper rightDropper;

WiFiClient client;
PubSubClient mqttClient(client);

void on_mqtt_message_received(char* topic, byte* payload, unsigned int length) {
  ESP_LOGD(TAG, "Message arrived in topic: %s, length %d", topic, length);
  if (length > 0) {
    ESP_LOGE(TAG, "Invalid mqtt message");
    return;
  }

  if (strcmp(topic, release_cube_0_topic) == 0) {
    if (leftDropper.isEmpty()) {
      ESP_LOGE(TAG, "Requested cube dropper is empty!");
      return;
    }

    leftDropper.releaseCube();
    ESP_LOGD(TAG, "Cube 0 has been released");
    return;
  }

  if (strcmp(topic, release_cube_1_topic) == 0) {
    // if (rightDropper.isEmpty()) {
    //   ESP_LOGE(TAG, "Requested cube dropper is empty!");
    //   return;
    // }

    // rightDropper.releaseCube();
    // ESP_LOGD(TAG, "Cube 1 has been released");
    // return;
  }

  if (strcmp(topic, insert_request_cube_0_topic) == 0) {
    if (!leftDropper.isEmpty()) {
      ESP_LOGE(TAG, "Cube dropper is already occupied!");
      return;
    }

    // wait for cube to be inserted
    bool inserted = leftDropper.waitForCubeInsertion();
    if (inserted) {
      ESP_LOGD(TAG, "Cube has been inserted");
    } else {
      ESP_LOGD(TAG, "Cube has NOT been inserted");
    }
    return;
  }

  if (strcmp(topic, insert_request_cube_1_topic) == 0) {
    // if (!rightDropper.isEmpty()) {
    //   ESP_LOGE(TAG, "Cube dropper is already occupied!");
    //   return;
    // }

    // // wait for cube to be inserted
    // ESP_LOGD(TAG, "Waiting for cube to be inserted");
    // while (digitalRead(rightIrPin) == HIGH) {
    //   ESP_LOGD(TAG, ".");
    //   delay(150);
    // }

    // rightDropper.onCubeInserted();
    return;
  }
}

void mqtt_connect() {
  while (!mqttClient.connected()) {
    ESP_LOGD(TAG, "Attempting MQTT connection");
    if (mqttClient.connect("pickup-point-0")) {
      ESP_LOGD(TAG, "MQTT connection established");
      mqttClient.subscribe(release_cube_0_topic);
      mqttClient.subscribe(insert_request_cube_0_topic);
      mqttClient.subscribe(release_cube_1_topic);
      mqttClient.subscribe(insert_request_cube_1_topic);
    } else {
      ESP_LOGD(TAG, "MQTT connection failed. Try again in %d seconds", WAIT_TIME_BEFORE_RECONNECT / 1000);
      delay(WAIT_TIME_BEFORE_RECONNECT);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(leftIrPin, INPUT);
  pinMode(rightIrPin, INPUT);

  leftDropper.init(LEFT_DROPPER_POSITION, leftServoPin, leftIrPin);
  rightDropper.init(RIGHT_DROPPER_POSITION, rightServoPin, rightIrPin);

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

  // if (digitalRead(leftIrPin) == LOW) {
  //   Serial.println("no");
  // } else {
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