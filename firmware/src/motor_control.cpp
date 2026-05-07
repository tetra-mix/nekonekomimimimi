#include "motor_control.h"

#include <Arduino.h>
#include <esp32-hal-ledc.h>

namespace {

constexpr unsigned long kOscTimeoutMs = 1000;
constexpr uint32_t kPwmFrequencyHz = 10000;
constexpr uint8_t kPwmResolutionBits = 8;
constexpr uint32_t kPwmMaxDuty = (1u << kPwmResolutionBits) - 1u;

constexpr int kMotorPins[kMotorCount] = {
  2,   // ch1 -> GPIO2 / D0
  3,   // ch2 -> GPIO3 / D1
  4,   // ch3 -> GPIO4 / D2
  5,   // ch4 -> GPIO5 / D3
  10,  // ch5 -> GPIO10 / D10
  9,   // ch6 -> GPIO9 / D9
  8    // ch7 -> GPIO8 / D8
};

constexpr int kPwmChannels[kMotorCount] = {0, 1, 2, 3, 4, 5, 6};

float motorLevels[kMotorCount] = {0.0f};
unsigned long lastOscReceivedMs = 0;
bool oscFailsafeTriggered = false;

float clampLevel(float value) {
  if (value < 0.0f) return 0.0f;
  if (value > 1.0f) return 1.0f;
  return value;
}

uint32_t levelToDuty(float level) {
  const float clamped = clampLevel(level);
  return static_cast<uint32_t>(clamped * static_cast<float>(kPwmMaxDuty) + 0.5f);
}

void applyMotorLevelToHardware(size_t index, float level) {
  ledcWrite(kMotorPins[index], levelToDuty(level));
}

}  // namespace

void initMotors() {
  for (size_t i = 0; i < kMotorCount; ++i) {
    pinMode(kMotorPins[i], OUTPUT);
    ledcAttachChannel(kMotorPins[i], kPwmFrequencyHz, kPwmResolutionBits, kPwmChannels[i]);
    ledcWrite(kMotorPins[i], 0);
    motorLevels[i] = 0.0f;
  }
}

void setMotorLevel(size_t index, float level) {
  if (index >= kMotorCount) {
    return;
  }

  const float clamped = clampLevel(level);
  motorLevels[index] = clamped;
  applyMotorLevelToHardware(index, clamped);

  Serial.printf("[motor] ch%u = %.3f\n", static_cast<unsigned>(index + 1), clamped);
}

void stopAllMotors() {
  for (size_t i = 0; i < kMotorCount; ++i) {
    motorLevels[i] = 0.0f;
    applyMotorLevelToHardware(i, 0.0f);
  }

  Serial.println("[motor] all stop");
}

void markOscReceived() {
  lastOscReceivedMs = millis();
  oscFailsafeTriggered = false;
}

void updateMotorFailsafe() {
  if (lastOscReceivedMs == 0 || oscFailsafeTriggered) {
    return;
  }

  if (millis() - lastOscReceivedMs < kOscTimeoutMs) {
    return;
  }

  stopAllMotors();
  oscFailsafeTriggered = true;
  Serial.println("[motor] failsafe stop: OSC timeout");
}
