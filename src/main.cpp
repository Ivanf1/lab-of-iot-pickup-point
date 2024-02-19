#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "wifikeys.h"
#include <Dropper/Dropper.h>

#include <esp_log.h>

#define TAG                "MAIN"

#define PICKUP_POINT_N_STR "0"
static const int PICKUP_POINT_N = 0;

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

static const char* release_request_cube_0_topic = "sm_iot_lab/pickup_point/0/cube/0/release_request";
static const char* release_response_cube_0_topic = "sm_iot_lab/pickup_point/0/cube/0/release_response";

static const char* release_request_cube_1_topic = "sm_iot_lab/pickup_point/0/cube/1/release_request";
static const char* release_response_cube_1_topic = "sm_iot_lab/pickup_point/0/cube/1/release_response";

static const char* insert_request_cube_topic = "sm_iot_lab/pickup_point/0/cube/insert_request";
static const char* insert_response_cube_topic = "sm_iot_lab/pickup_point/0/cube/insert_response";

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

  if (strcmp(topic, release_request_cube_0_topic) == 0) {
    if (leftDropper.isEmpty()) {
      ESP_LOGE(TAG, "Requested cube dropper 0 is empty!");
      return;
    }
    StaticJsonDocument<200> doc;
    doc["pickupPointN"] = PICKUP_POINT_N;
    doc["cubeDropperN"] = LEFT_DROPPER_POSITION;
    size_t doc_size = measureJson(doc);
    uint8_t* output = (uint8_t*)malloc(doc_size);
    serializeJson(doc, (void*)output, doc_size);

    leftDropper.releaseCube();
    ESP_LOGD(TAG, "Cube 0 has been released");
    mqttClient.publish(release_response_cube_0_topic, output, doc_size);
    free(output);
    return;
  }

  if (strcmp(topic, release_request_cube_1_topic) == 0) {
    if (rightDropper.isEmpty()) {
      ESP_LOGE(TAG, "Requested cube dropper 1 is empty!");
      return;
    }
    rightDropper.releaseCube();
    ESP_LOGD(TAG, "Cube 1 has been released");

    StaticJsonDocument<200> doc;
    doc["pickupPointN"] = PICKUP_POINT_N;
    doc["cubeDropperN"] = RIGHT_DROPPER_POSITION;
    size_t doc_size = measureJson(doc);
    uint8_t* output = (uint8_t*)malloc(doc_size);
    serializeJson(doc, (void*)output, doc_size);

    mqttClient.publish(release_response_cube_1_topic, output, doc_size);
    free(output);
    return;
  }

  if (strcmp(topic, insert_request_cube_topic) == 0) {
    StaticJsonDocument<200> doc;
    size_t doc_size;
    uint8_t* output;
    doc["pickupPointN"] = PICKUP_POINT_N;
    doc["cubeId"] = (char*)payload;

    if (leftDropper.isEmpty()) {
      doc["cubeDropperN"] = LEFT_DROPPER_POSITION;
      // wait for cube to be inserted
      bool inserted = leftDropper.cubeInsertionRequest();
      if (inserted) {
        ESP_LOGD(TAG, "Cube has been inserted in left dropper");
        doc["status"] = "inserted";

      } else {
        ESP_LOGD(TAG, "Cube could NOT be inserted in left dropper");
        doc["status"] = "error";
      }

      doc_size = measureJson(doc);
      output = (uint8_t*)malloc(doc_size);
      serializeJson(doc, (void*)output, doc_size);

      mqttClient.publish(insert_response_cube_topic, output, doc_size);
      free(output);

      return;
    }

    if (rightDropper.isEmpty()) {
      doc["cubeDropperN"] = RIGHT_DROPPER_POSITION;
      // wait for cube to be inserted
      bool inserted = rightDropper.cubeInsertionRequest();
      if (inserted) {
        ESP_LOGD(TAG, "Cube has been inserted in right dropper");
        doc["status"] = "inserted";
      } else {
        ESP_LOGD(TAG, "Cube could NOT be inserted in right dropper");
        doc["status"] = "error";
      }

      doc_size = measureJson(doc);
      output = (uint8_t*)malloc(doc_size);
      serializeJson(doc, (void*)output, doc_size);

      mqttClient.publish(insert_response_cube_topic, output, doc_size);
      free(output);

      return;
    }

    ESP_LOGE(TAG, "Cube Droppers are all already full!");

    doc["cubeDropperN"] = -1;
    doc["status"] = "all full";
    doc_size = measureJson(doc);
    output = (uint8_t*)malloc(doc_size);
    serializeJson(doc, (void*)output, doc_size);

    mqttClient.publish(insert_response_cube_topic, output, doc_size);
    free(output);
    return;
  }
}

void mqtt_connect() {
  while (!mqttClient.connected()) {
    ESP_LOGD(TAG, "Attempting MQTT connection");
    if (mqttClient.connect(
            "pickup-point-" PICKUP_POINT_N_STR, "sm_iot_lab/pickup_point/" PICKUP_POINT_N_STR "/status", 2, true, "down"
        )) {
      ESP_LOGD(TAG, "MQTT connection established");
      mqttClient.subscribe(release_request_cube_0_topic);
      mqttClient.subscribe(release_request_cube_1_topic);
      mqttClient.subscribe(insert_request_cube_topic);
      mqttClient.publish("sm_iot_lab/pickup_point/" PICKUP_POINT_N_STR "/status", "up", true);
    } else {
      ESP_LOGD(TAG, "MQTT connection failed. Try again in %d seconds", WAIT_TIME_BEFORE_RECONNECT / 1000);
      delay(WAIT_TIME_BEFORE_RECONNECT);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(leftIrPin, INPUT);
  pinMode(leftRedLedPin, OUTPUT);
  pinMode(leftYellowLedPin, OUTPUT);
  pinMode(leftGreenLedPin, OUTPUT);
  leftDropper.init(LEFT_DROPPER_POSITION, leftServoPin, leftIrPin, leftRedLedPin, leftYellowLedPin, leftGreenLedPin);

  pinMode(rightIrPin, INPUT);
  pinMode(rightRedLedPin, OUTPUT);
  pinMode(rightYellowLedPin, OUTPUT);
  pinMode(rightGreenLedPin, OUTPUT);
  rightDropper.init(
      RIGHT_DROPPER_POSITION, rightServoPin, rightIrPin, leftRedLedPin, leftYellowLedPin, leftGreenLedPin
  );

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  mqttClient.setServer(BROKER_IP, BROKER_PORT);
  mqttClient.setCallback(on_mqtt_message_received);

  mqtt_connect();
}

void loop() {
  if (!mqttClient.connected()) {
    ESP_LOGD(TAG, "mqtt client disconnected");
    mqtt_connect();
  }
  mqttClient.loop();
}