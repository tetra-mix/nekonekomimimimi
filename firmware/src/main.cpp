#include <Arduino.h>
#include <ArduinoOSCWiFi.h>
#include <WiFi.h>

#define ESP_WM_LITE_DEBUG_OUTPUT Serial
#define _ESP_WM_LITE_LOGLEVEL_ 2
// The board pins used for the motors overlap typical status LED defaults.
#define USE_LED_BUILTIN false
// Accept a single Wi-Fi credential pair in the config portal.
#define REQUIRE_ONE_SET_SSID_PW true

#include <ESP_WiFiManager_Lite.h>
#include "motor_control.h"
#include "osc_router.h"

bool LOAD_DEFAULT_CONFIG_DATA = false;

ESP_WM_LITE_Configuration defaultConfig = {
  "ESP32",
  {
    {"", ""},
    {"", ""}
  },
  "hard-osc",
  0
};

uint32_t getChipID() {
  uint64_t chipId64 = 0;

  for (int i = 0; i < 6; ++i) {
    chipId64 |= (((uint64_t)ESP.getEfuseMac() >> (40 - (i * 8))) & 0xff) << (i * 8);
  }

  return static_cast<uint32_t>(chipId64 & 0xFFFFFF);
}

uint32_t getChipOUI() {
  uint64_t chipId64 = 0;

  for (int i = 0; i < 6; ++i) {
    chipId64 |= (((uint64_t)ESP.getEfuseMac() >> (40 - (i * 8))) & 0xff) << (i * 8);
  }

  return static_cast<uint32_t>(chipId64 >> 24);
}

namespace {

constexpr int kOscRecvPort = 9001;
constexpr char kHostName[] = "hard-osc";
constexpr char kConfigPortalSsid[] = "hard-osc-setup";
constexpr char kConfigPortalPassword[] = "00000000";

ESP_WiFiManager_Lite* wifiManager = nullptr;
bool lastConfigMode = false;

void setupWiFiManager() {
  wifiManager = new ESP_WiFiManager_Lite();
  wifiManager->setConfigPortal(kConfigPortalSsid, kConfigPortalPassword);
  wifiManager->setConfigPortalChannel(0);
  wifiManager->begin(kHostName);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("[wifi] connected, STA IP: ");
    Serial.println(WiFi.localIP());
  } else if (wifiManager->isConfigMode()) {
    Serial.print("[wifi] config portal active: ");
    Serial.println(kConfigPortalSsid);
  }
}

void updateWiFiManager() {
  wifiManager->run();

  const bool configMode = wifiManager->isConfigMode();
  if (configMode != lastConfigMode) {
    lastConfigMode = configMode;

    if (configMode) {
      Serial.print("[wifi] entered config portal: ");
      Serial.println(kConfigPortalSsid);
    } else if (WiFi.status() == WL_CONNECTED) {
      Serial.print("[wifi] connected, STA IP: ");
      Serial.println(WiFi.localIP());
    }
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(200);

  initMotors();
  stopAllMotors();

  setupWiFiManager();
  subscribeOscRoutes(kOscRecvPort);

  Serial.printf("[osc] listening on UDP %d\n", kOscRecvPort);
  Serial.println("[osc] routes: /motor/1..7, /motor/all, /motor/stop");
}

void loop() {
  updateWiFiManager();
  OscWiFi.update();
  updateMotorFailsafe();
  delay(0);
}
