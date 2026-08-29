#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>

BH1750 lightSensor;

char ssid[] = "TT";
char password[] = "12345678";

const char mqttServer[] =
  "8abfc976a0b944208bac1582ad0cb531.s1.eu.hivemq.cloud";

const int mqttPort = 8883;

const char mqttUsername[] = "arnav_mqtt";
const char mqttPassword[] = "ArnaV@2006";

const char mqttTopic[] = "arnav/sensors";

WiFiSSLClient wifiClient;
MqttClient mqttClient(wifiClient);

void connectWiFi()
{
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(ssid, password);

    delay(5000);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT()
{
  Serial.print("Connecting to HiveMQ");

  mqttClient.setUsernamePassword(
    mqttUsername,
    mqttPassword
  );

  while (!mqttClient.connect(mqttServer, mqttPort))
  {
    Serial.print(".");
    delay(5000);
  }

  Serial.println();
  Serial.println("Connected to HiveMQ!");
}

void setup()
{
  Serial.begin(9600);

  delay(2000);

  Serial.println();

  Serial.println("Light Sensor System Starting");


  
  Wire.begin();

  Serial.println("I2C started");

  if (lightSensor.begin())
  {
    Serial.println("BH1750 sensor ready!");
  }
  else
  {
    Serial.println("BH1750 sensor failed!");
  }

  connectWiFi();

  connectMQTT();

  Serial.println();
  Serial.println("System ready!");
}

void loop()
{
  
  if (!mqttClient.connected())
  {
    connectMQTT();
  }

  mqttClient.poll();

  float light = lightSensor.readLightLevel();

  if (light < 0)
  {
    Serial.println("Light reading failed!");

    delay(5000);

    return;
  }

  Serial.println();


  Serial.print("Light: ");
  Serial.print(light, 2);
  Serial.println(" lux");

  String message = "{";

  message += "\"light\":";
  message += String(light, 2);

  message += "}";

  Serial.print("MQTT message: ");
  Serial.println(message);

  mqttClient.beginMessage(mqttTopic);

  mqttClient.print(message);

  mqttClient.endMessage();


  Serial.println("Data sent to HiveMQ!");

  for (int i = 0; i < 30; i++)
  {
    mqttClient.poll();
    delay(1000);
  }
}


