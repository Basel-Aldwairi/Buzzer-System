#include <Arduino.h>

const int ledPin = 14;
const int buttonPin = 27;
const int buzzerPin = 26;
const int volumePotPin = 25;
const int frequencyPotPin = 33;

volatile bool systemState = false;

volatile long timer = 0;
volatile bool buzzerTurn = false;

long debounceTimer = 0;

void toggleSystem();

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(volumePotPin, INPUT);
  pinMode(frequencyPotPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleSystem, RISING);

  Serial.begin(115200);
}

void loop()
{
  if (systemState)
  {
    int volumeRead = analogRead(volumePotPin);
    int volume = map(volumeRead, 0, 4095, 0, 255);

    int frequencyRead = analogRead(frequencyPotPin);
    int delayTime = map(frequencyRead, 0, 4095, 20, 1000);

    if (millis() - timer >= delayTime)
    {
      if (buzzerTurn)
      {
        analogWrite(buzzerPin, volume);
        Serial.printf("V : %d, F : %d\n", volume, delayTime);
      }
      else
      {
        analogWrite(buzzerPin, 0);
      }
      buzzerTurn = !buzzerTurn;
      timer = millis();
    }
  }
}

void toggleSystem()
{
  if (millis() - debounceTimer >= 100)
  {
    systemState = !systemState;
    digitalWrite(ledPin, systemState);
    analogWrite(buzzerPin, 0);
    buzzerTurn = true;
    timer = millis();
    debounceTimer = millis();
  }
}
