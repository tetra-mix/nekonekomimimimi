#include <Arduino.h>

void setup() {
  constexpr uint8_t kMotorPin = D10;

  Serial.begin(115200);
  delay(500);

  pinMode(kMotorPin, OUTPUT);
  digitalWrite(kMotorPin, LOW);

  Serial.println("MTR5 test start");
}

void loop() {
  constexpr uint8_t kMotorPin = D10;
  constexpr unsigned long kOnMs = 1000;
  constexpr unsigned long kOffMs = 1000;

  Serial.println("MTR5 ON");
  digitalWrite(kMotorPin, HIGH);
  delay(kOnMs);

  Serial.println("MTR5 OFF");
  digitalWrite(kMotorPin, LOW);
  delay(kOffMs);
}
