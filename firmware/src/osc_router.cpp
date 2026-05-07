#include "osc_router.h"

#include <ArduinoOSCWiFi.h>

#include "motor_control.h"

namespace {

void handleMotorMessage(size_t index, const OscMessage &message) {
  markOscReceived();
  const float level = message.size() ? message.arg<float>(0) : 0.0f;
  setMotorLevel(index, level);
}

void handleMotorAllMessage(const OscMessage &message) {
  markOscReceived();
  const float level = message.size() ? message.arg<float>(0) : 0.0f;

  for (size_t i = 0; i < kMotorCount; ++i) {
    setMotorLevel(i, level);
  }
}

void handleMotorStopMessage(const OscMessage &) {
  markOscReceived();
  stopAllMotors();
}

}  // namespace

void subscribeOscRoutes(int port) {
  OscWiFi.subscribe(port, "/motor/1", [](const OscMessage &m) { handleMotorMessage(0, m); });
  OscWiFi.subscribe(port, "/motor/2", [](const OscMessage &m) { handleMotorMessage(1, m); });
  OscWiFi.subscribe(port, "/motor/3", [](const OscMessage &m) { handleMotorMessage(2, m); });
  OscWiFi.subscribe(port, "/motor/4", [](const OscMessage &m) { handleMotorMessage(3, m); });
  OscWiFi.subscribe(port, "/motor/5", [](const OscMessage &m) { handleMotorMessage(4, m); });
  OscWiFi.subscribe(port, "/motor/6", [](const OscMessage &m) { handleMotorMessage(5, m); });
  OscWiFi.subscribe(port, "/motor/7", [](const OscMessage &m) { handleMotorMessage(6, m); });
  OscWiFi.subscribe(port, "/motor/all", handleMotorAllMessage);
  OscWiFi.subscribe(port, "/motor/stop", handleMotorStopMessage);
}
