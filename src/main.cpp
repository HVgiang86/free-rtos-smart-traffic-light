#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "LEDController.h"
#include "TrafficLight.h"
#include "TrafficControl.h"

#define DIO 14  // DS
#define RCLK 12 // SHCP
#define SCLK 13 // STCP

#define G_A 25
#define R_A 27
#define Y_A 26
#define G_B 19
#define R_B 5
#define Y_B 18

LEDController ledController(DIO, SCLK, RCLK);

/// WIFI Settings ///
const char *ssid = "Trung Giang";
const char *password = "24062002";

/// Config MQTT ///

#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_USER "mqtt"
#define MQTT_PASSWORD "mqtt"

#define MQTT_TOPIC "mqtt/hvgiang/topic"

volatile unsigned long prevTime = 0;

TaskHandle_t count_handle;
TaskHandle_t receiveQueue_handle;

void reconnect();

// Queue to store JSON data
QueueHandle_t jsonQueue;

WiFiClient client;
PubSubClient mqttClient(client);

TrafficLight lightA;
TrafficLight lightB;

TrafficControl controller(lightA, lightB);

// Callback function to handle incoming messages
void callback(char *topic, byte *payload, unsigned int length)
{
    payload[length] = '\0'; // Null-terminate the payloay
    String message = String((char *)payload);

    xQueueSend(jsonQueue, message.c_str(), portMAX_DELAY);
}

void socketIOTask(void *pvParameters);
void queueHandleTask(void *pvParameters);
void displayTask(void *pvParameters);
void autoCountTask(void *pvParameters);

// The task that will handle the Socket.IO loop
void socketIOTask(void *pvParameters)
{
    (void)pvParameters;

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Set the MQTT server and callback function
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(callback);

    for (;;)
    {
        // Ensure the client is connected to the broker
        if (!mqttClient.connected())
        {
            reconnect();
        }
        mqttClient.loop();
        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to avoid overloading the CPU
    }
}

void queueHandleTask(void *pvParameters)
{
    (void)pvParameters;

    char messageBuffer[60];
    for (;;)
    {
        if (xQueueReceive(jsonQueue, &messageBuffer, portMAX_DELAY) == pdPASS)
        {
            String msg(messageBuffer);
            msg = msg.substring(1, msg.length() - 1); // Remove quotes
            msg.replace("\\", "");
            // Serial.println(msg);

            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc,msg.c_str());

            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                continue;
            }

            // Extract values
            int r1 = doc["r1"];
            int r2 = doc["r2"];
            int r3 = doc["r3"];
            int r4 = doc["r4"];

            // Serial.print(r1);
            // Serial.print(" ");
            // Serial.println(r2);

            vTaskSuspend(count_handle);
            controller.updateDuration(r1 * 1000, r2 * 1000);
            vTaskResume(count_handle);
        }
    }
}

void autoCountTask(void *pvParameters)
{
    for (;;)
    {
        if (millis() - prevTime >= 20)
        {
            controller.update();

            prevTime = millis();
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void displayTask(void *pvParameters)
{
    for (;;)
    {
        int timeA = (int)(lightA.getRemainingTime() / 1000) % 100;
        int timeB = (int)(lightB.getRemainingTime() / 1000) % 100;

        int t = (timeA * 100 + timeB) % 10000;

        ledController.display(timeA, timeB, timeA, timeB);
        // 1 - 4 - 3 - 2
        // ledController.display(1, 2, 3, 4);

        digitalWrite(G_A, LOW);
        digitalWrite(R_A, LOW);
        digitalWrite(Y_A, LOW);

        if (lightA.getState() == GREEN)
            digitalWrite(G_A, HIGH);
        else if (lightA.getState() == RED)
            digitalWrite(R_A, HIGH);
        else
            digitalWrite(Y_A, HIGH);

        digitalWrite(G_B, LOW);
        digitalWrite(R_B, LOW);
        digitalWrite(Y_B, LOW);

        if (lightB.getState() == GREEN)
            digitalWrite(G_B, HIGH);
        else if (lightB.getState() == RED)
            digitalWrite(R_B, HIGH);
        else
            digitalWrite(Y_B, HIGH);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    Serial.begin(9600);
    ledController.begin();
    controller.init();

    pinMode(G_A, OUTPUT);
    pinMode(R_A, OUTPUT);
    pinMode(Y_A, OUTPUT);
    pinMode(G_B, OUTPUT);
    pinMode(R_B, OUTPUT);
    pinMode(Y_B, OUTPUT);

    jsonQueue = xQueueCreate(10, sizeof(char) * 60);

    xTaskCreate(socketIOTask, "Task SocketIo!", 4096, NULL, 1, NULL);
    xTaskCreate(autoCountTask, "Task autocount!", 4096, NULL, 1, &count_handle);
    xTaskCreate(displayTask, "Task display!", 4096, NULL, 1, NULL);
    xTaskCreate(queueHandleTask, "Task handle queue!", 4096, NULL, 1, NULL);
}

// We don't need to use "loop" function here!
void loop()
{
    // ledController.display(0, 0, 0, 5555);
    // 1 - 4 - 3 - 2
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (mqttClient.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, subscribe to the topic
            mqttClient.subscribe(MQTT_TOPIC);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}
