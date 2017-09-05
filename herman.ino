#include "Lx200Communication.h"
#include "Lx200Protocol.h"

Lx200Protocol* lx200Protocol;
Telescope* telescope;

const int ledPin =  LED_BUILTIN;// the number of the LED pin
int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  pinMode(ledPin, OUTPUT);

  // Telescope initialization
  telescope = new Telescope ();
  
  // Communication and protocol initialization
  Lx200Communication* lx200Communication = new Lx200Communication();
  lx200Protocol = new Lx200Protocol(lx200Communication, telescope);

}

void loop() {
  
  // Handle protocol message
  lx200Protocol->HandleCommunication ();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }

}
