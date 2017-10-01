#include "Lx200Communication.h"
#include "Lx200Protocol.h"

Lx200Protocol* lx200Protocol = NULL;
Lx200Protocol* lx200Protocol2 = NULL;
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
  Lx200Communication* lx200Communication = new Lx200Communication(Lx200Communication::SERIAL0);
  lx200Protocol = new Lx200Protocol(lx200Communication, telescope);

  if (Lx200Communication::IsDeviceSupported(Lx200Communication::SERIAL1)) {
    Lx200Communication* lx200Communication2 = new Lx200Communication(Lx200Communication::SERIAL1);
    lx200Protocol2 = new Lx200Protocol(lx200Communication2, telescope);    
  }

}

void loop() {

  // Handle protocol message
  lx200Protocol->HandleCommunication ();
  if (lx200Protocol != NULL) {
    lx200Protocol2->HandleCommunication ();    
  }

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
