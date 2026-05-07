#pragma once

#include <stddef.h>

constexpr size_t kMotorCount = 7;

void initMotors();
void setMotorLevel(size_t index, float level);
void stopAllMotors();
void markOscReceived();
void updateMotorFailsafe();
