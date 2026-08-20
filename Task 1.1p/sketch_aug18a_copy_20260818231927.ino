// Pins for the lights and the button
const int porchLight = 2;
const int hallwayLight = 3;
const int arrivalButton = 4;

void setup() {
  // Set the lights as outputs and the button as an input
  pinMode(porchLight, OUTPUT);
  pinMode(hallwayLight, OUTPUT);
  pinMode(arrivalButton, INPUT);

  Serial.begin(9600);
}

void loop() {
  // Check if Linda has pressed the button
  int buttonState = digitalRead(arrivalButton);

  if (buttonState == HIGH) {
    Serial.println("Linda has arrived! Turning on the lights...");
    startLights();
  }
}

// Handles the timing for the porch and hallway lights
void startLights() {
  // Turn both lights on when Linda arrives
  digitalWrite(porchLight, HIGH);
  digitalWrite(hallwayLight, HIGH);

  Serial.println("Both lights are ON.");

  // Keep both lights on for 30 seconds
  delay(30000);

  // Turn the porch light off after 30 seconds
  digitalWrite(porchLight, LOW);

  Serial.println("Porch light is OFF.");

  // Keep the hallway light on for another 30 seconds
  delay(30000);

  // Turn the hallway light off after one minute
  digitalWrite(hallwayLight, LOW);

  Serial.println("Hallway light is OFF. Done.");
}