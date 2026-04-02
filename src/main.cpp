// Basel Al-Dwairi
// 20221502065
// ESD Assignment 1

// Originally designed and programmed on a real ESP32 (DEVKIT1)
#include <Arduino.h>

// Pin aliasing
const int ledPin = 14;
const int buttonPin = 27;
const int buzzerPin = 26;
const int volumePotPin = 25;
const int frequencyPotPin = 33;

// Controlable state
volatile bool systemState = false;

// Buzzer varialbles
volatile long timer = 0;
volatile bool buzzerTurn = false;

// Debounce variables
long debounceTimer = 0;
const long debounceWait = 100;

// Function declaration
void toggleSystem();


// Main code
void setup()
{
  // Initialize pins
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(volumePotPin, INPUT);
  pinMode(frequencyPotPin, INPUT);

  // Interrupts
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleSystem, RISING);

  // Serial for debugging
  Serial.begin(115200);
}

void loop()
{
  // Only execute code if system is on
  if (systemState)
  {
    // Read pots and map to variables
    int volumeRead = analogRead(volumePotPin);
    int volume = map(volumeRead, 0, 4095, 0, 255);

    // 20221502065 -> 65 for he minimum delay
    int frequencyRead = analogRead(frequencyPotPin);
    int delayTime = map(frequencyRead, 0, 4095, 65, 1000);

    // If delay is over (non-blocking)
    if (millis() - timer >= delayTime)
    {
      // If buzzer should be on or off for this period
      if (buzzerTurn)
      {
        // Write to the buzzer
        analogWrite(buzzerPin, volume);
        // Debugging
        Serial.printf("V : %d, F : %d, VR : %d\n", volume, delayTime, volumeRead);
      }
      else
      {
        // Turn off the buzzer
        analogWrite(buzzerPin, 0);
      }
      // Switch turns for next cycle and get new time
      buzzerTurn = !buzzerTurn;
      timer = millis();
    }
  }
}


// Function definition
void toggleSystem()
{
  // Non-blocking delay to handle debounce
  if (millis() - debounceTimer >= debounceWait)
  {
    // Toggle system state and show with LED 
    systemState = !systemState;
    digitalWrite(ledPin, systemState);

    // Turn off buzzer and ready the next cycle
    analogWrite(buzzerPin, 0);
    buzzerTurn = true;
    timer = millis();
    
    // Handle debounce 
    debounceTimer = millis();
  }
}
