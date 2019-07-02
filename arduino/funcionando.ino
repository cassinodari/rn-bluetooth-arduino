#include "BluetoothSerial.h"

BluetoothSerial bluetooth;

int speakerPin = 2;
int ledBluePin = 26;
int statusBlue = 0;
int ledRedPin = 33;
int statusRed = 0;
int ledYellowPin = 25;
int statusYellow = 0;
int ledGreenPin = 32;
int statusGreen = 0;
int bombaPin = 4;
int r = 1;
int statusSpeaker = 0;
int statusLed = 1;
int statusBomb = 0;

int length = 15; // the number of notes
char notes[] = "ccggaagffeeddc "; // a space represents a rest
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

void startLed(int led) {
  Serial.println(led);
  if (led == 48 and statusGreen == 1) {
      statusGreen = 0;
      soLeds(led, statusGreen);
    } else if (led == 48 and statusGreen == 0) {
      statusGreen = 1;
      soLeds(led, statusGreen);
    } else if (led == 49 and statusRed == 1) {
      statusRed = 0;
      soLeds(led, statusRed);
    } else if (led == 49 and statusRed == 0) {
      statusRed = 1;
      soLeds(led, statusRed);
    } else if (led == 50 and statusBlue == 1) {
      statusBlue = 0;
      soLeds(led, statusBlue);
    } else if (led == 50 and statusBlue == 0) {
      statusBlue = 1;
      soLeds(led, statusBlue);
    } else if (led == 51 and statusYellow == 1) {
      statusYellow = 0;
      soLeds(led, statusYellow);
    } else if (led == 51 and statusYellow == 0) {
      statusYellow = 1;
      soLeds(led, statusYellow);
    }
}

void soLeds(int statusLeds, int op) {
  // Liga
  if (op == 1) {
    if (statusLeds == 48 || statusLeds == 1) {
      digitalWrite(ledGreenPin, HIGH);        
    }
    
    if (statusLeds == 49 || statusLeds == 1) {
      digitalWrite(ledRedPin, HIGH);
    }
    
    if (statusLeds == 50 || statusLeds == 1) {
      digitalWrite(ledBluePin, HIGH);
    }
    
    if (statusLeds == 51 || statusLeds == 1) {
      digitalWrite(ledYellowPin, HIGH);
    }
  } else {
    // Desliga
    if (statusLeds == 48 || statusLeds == 1) {
      digitalWrite(ledGreenPin, LOW);        
    }
    if (statusLeds == 49 || statusLeds == 1) {
      digitalWrite(ledRedPin, LOW);
    }
    
    if (statusLeds == 50 || statusLeds == 1) {
      digitalWrite(ledBluePin, LOW);
    }
    
    if (statusLeds == 51 || statusLeds == 1) {
      digitalWrite(ledYellowPin, LOW);
    }
  }

  
}

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
      if (bluetooth.available()) {
        r = bluetooth.read();
        statusLed = r;
      } else {
        r = statusLed;
      }
      
      digitalWrite(speakerPin, HIGH);
      // Liga leds
      soLeds(r, 1);
      
      delayMicroseconds(tone);
      
      digitalWrite(speakerPin, LOW);
      // Desliga leds
      soLeds(r, 0);
      
      delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
  Serial.begin(38400);
  Serial.println(("Type the AT commands:"));
  
  pinMode(speakerPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledYellowPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(bombaPin, OUTPUT);
  bluetooth.begin("ESP32");
}

void loop() {
    if (bluetooth.available()) {
      r = bluetooth.read();
    } else {
      r = 1;
    }
    
    if (r == 53 && statusSpeaker == 0) {
      statusSpeaker = 1;
      statusLed = 1;
      for (int i = 0; i < length; i++) {
        if (notes[i] == ' ') {
          delay(beats[i] * tempo); // rest
        } else {
          digitalWrite(bombaPin, LOW);
          playNote(notes[i], beats[i] * tempo);
        }
        // pause between notes
        digitalWrite(bombaPin, HIGH);
        delay(tempo / 2);
      }
    } else if (r == 53 and statusSpeaker == 1) {
      statusSpeaker = 0;
      statusLed = 0;
      if (bluetooth.available()) {
        r = bluetooth.read();
      }
      startLed(r);
      
    } else if (r == 52) {
      if (statusBomb == 0) {
        statusBomb = 1;
        digitalWrite(bombaPin, LOW);
      } else {
        statusBomb = 0;
        digitalWrite(bombaPin, HIGH);
      }
    } else {
      statusLed = 0;
      startLed(r);
    }
  
}
