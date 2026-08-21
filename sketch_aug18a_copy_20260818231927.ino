#include <Wire.h>
#include <DHT.h>
#include <BH1750.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

// setting pins and defining type for dht sensor
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightsensor;

// id and pass for my wifi
char ssid[] = "Arnav's iphone";
char password[] = "11221122";

// channel id and api key from thing speak
unsigned long channelID = 3464955;
const char* writeAPIKey = "NQSNWLLGLEW57V8G";

WiFiClient client;

// this function helps in connecting to wifi
void connectWiFi() {

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    WiFi.begin(ssid, password);

    delay(5000);

    Serial.print(".");
  }
// prints connected if it works
  Serial.println();
  Serial.println("WiFi connected!");
}

// funtion to read temperature
float readTemperature() {

  float temperature = dht.readTemperature();
  //returns the temperature
  return temperature;
}

// function to read light level
float readLight() {

  float light = lightsensor.readLightLevel();
  //returns the light level
  return light;
}

// this function sends data to thing speak 
void sendData(float temperature, float light) {

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, light);

  int response = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (response == 200) {
    Serial.println("Data successfully sent to ThingSpeak!");
  } 
  else { //this else helps to tell if any problem accured while connecting
    Serial.print("ThingSpeak error: ");
    Serial.println(response);
  }
}

// Setup code starting everything
void setup() {
//defining serial port
  Serial.begin(9600);

  dht.begin();

  Wire.begin();
  lightsensor.begin();

  ThingSpeak.begin(client);

  connectWiFi();

  Serial.println("System ready!");
}

// here every funtion is called 
void loop() {

  float temperature = readTemperature();
  float light = readLight();

// this prints temp and light level in serial monitor
  Serial.println("");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Light: ");
  Serial.print(light);
  Serial.println(" lux");

// exeption handelling
  if (isnan(temperature)) {

    Serial.println("Temperature reading failed!");

  } 
  else {

    sendData(temperature, light);
  }

  Serial.println("Wait 30 seconds");

  delay(30000);
}