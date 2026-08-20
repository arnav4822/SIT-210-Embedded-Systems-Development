                  Task 1.1P

1. PROJECT OVERVIEW
This project automates home lights for Linda using an Arduino. 
When she presses the button, the porch and hallway lights turn on. 
The lights then turn off one by one to save energy.


2. HARDWARE SETUP
- Porch Light (LED)   : Pin 2 (porchLight)
- Hallway Light (LED) : Pin 3 (hallwayLight)
- Push Button         : Pin 4 (arrivalButton)


3. HOW THE CODE WORKS
I kept the code simple and modular by splitting it into two parts:

- Setup:
  The setup() function sets the LED pins as outputs. It sets the 
  button pin as an input. It also starts the Serial Monitor at 9600 
  speed to print status messages.

- Main Loop:
  The loop() function only watches the button. When someone presses 
  it, the code prints a message and calls the startLights() function.

- Lighting Steps (startLights()):
  1. Both lights turn on right away.
  2. The code waits for 30 seconds.
  3. The porch light turns off.
  4. The code waits for another 30 seconds.
  5. The hallway light turns off. It stays on for 60 seconds total.


4. PROBLEMS WITH USING delay()
Using delay() is easy, but it causes big problems:

- The Board Freezes:
  When delay() runs, the Arduino completely pauses. It cannot do any 
  other work while waiting.

- Missed Button Presses:
  If Linda presses the button again during the 60 seconds, the board 
  ignores it. It will not see any new clicks until the timer ends.

- Hard to Add New Parts:
  We cannot easily add new sensors, like a motion detector or door 
  alarm. The board cannot check two things at the same time.

To fix this for a real smart home, we should use millis() instead.
