#include <WiFiNINA.h>
#include <PubSubClient.h>

// ---------- Wi-Fi ----------
const char* ssid = "Anmol4G";
const char* password = "Pineapple@11205";

// ---------- MQTT ----------
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;

// Put your name here
const char* myName = "Arnav";

// ---------- Pins ----------
const int trigPin = 2;
const int echoPin = 3;

const int bathroomLED = 12;
const int hallwayLED = 11;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


// ---------- Ultrasonic distance ----------
long getDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  long distance = duration * 0.034 / 2;

  return distance;
}


// ---------- MQTT message received ----------
void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message received on ");
  Serial.print(topic);
  Serial.print(": ");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();

  // Wave -> turn both LEDs ON
  if (strcmp(topic, "ES/Wave") == 0)
  {
    digitalWrite(bathroomLED, HIGH);
    digitalWrite(hallwayLED, HIGH);
  }

  // Pat -> turn both LEDs OFF
  if (strcmp(topic, "ES/Pat") == 0)
  {
    digitalWrite(bathroomLED, LOW);
    digitalWrite(hallwayLED, LOW);
  }
}


// ---------- MQTT connection ----------
void reconnectMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.print("Connecting to MQTT...");

    String clientID = "ArduinoClient-";
    clientID += String(random(0xffff), HEX);

    if (mqttClient.connect(clientID.c_str()))
    {
      Serial.println("connected");

      mqttClient.subscribe("ES/Wave");
      mqttClient.subscribe("ES/Pat");

      Serial.println("Subscribed to ES/Wave");
      Serial.println("Subscribed to ES/Pat");
    }
    else
    {
      Serial.print("Failed, rc=");
      Serial.println(mqttClient.state());

      delay(5000);
    }
  }
}


// ---------- Setup ----------
void setup()
{
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(bathroomLED, OUTPUT);
  pinMode(hallwayLED, OUTPUT);

  digitalWrite(bathroomLED, LOW);
  digitalWrite(hallwayLED, LOW);

  // Connect Wi-Fi
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.begin(ssid, password) != WL_CONNECTED)
  {
    Serial.print(".");
    delay(3000);
  }

  Serial.println();
  Serial.println("Wi-Fi connected");

  // MQTT setup
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
}


// ---------- Main loop ----------
void loop()
{
  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }

  mqttClient.loop();

  long distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // -------- Wave detection --------
  // Example definition:
  // object/hand comes between 10 and 30 cm
  if (distance >= 10 && distance <= 30)
  {
    Serial.println("WAVE DETECTED");

    mqttClient.publish("ES/Wave", myName);

    delay(2000);
  }

  // -------- Pat detection --------
  // Example definition:
  // very close object/hand
  if (distance > 0 && distance < 10)
  {
    Serial.println("PAT DETECTED");

    mqttClient.publish("ES/Pat", myName);

    delay(2000);
  }

  delay(100);
}


