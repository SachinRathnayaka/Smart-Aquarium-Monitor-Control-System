/*
  ===============================================================
  IoT Based Smart Aquarium Monitor & Control System (ESP32) - Production
  ===============================================================

  Hardware mapping strictly follows your latest schematic request:

  - pH Sensor (Analog)                 : GPIO 34
  - TDS Sensor (Analog)                : GPIO 35
  - Turbidity Sensor (Analog)          : GPIO 32
  - DS18B20 Temp Sensor (OneWire)      : GPIO 25
  - BMS Battery Sense (Analog)         : GPIO 33
  - Cooling Fan Relay                  : GPIO 26
  - Water Pump Relay                   : GPIO 27
  - Heater Relay                       : GPIO 17
  - Fish Feeder Servo                  : GPIO 23
  - ARGB LED Data                      : GPIO 16
  - Buzzer                             : GPIO 13
  - I2C SDA                            : GPIO 21
  - I2C SCL                            : GPIO 22
  - Wi-Fi/Firebase Status LED          : GPIO 4

  

  Key features:
  - Fully non-blocking millis() architecture in loop()
  - DS3231 RTC is the only time source for time-based behavior (offline-safe)
  - Offline auto mode continues sensor/read/control/LCD even without Wi-Fi/Firebase
  - ARGB day-cycle lighting via RTC (morning/day/night breathing)
  - Firebase RTDB control stream + dedicated /commands voice hooks
  - Local web dashboard (port 80) + fish feeding button
  - Filter change alert after 2h pump runtime due to turbidity not improving
  - Auto water-change sequence on critical TDS (max 8 minutes safeguard)
  - Turbidity SMA + degradation trend prediction (days to maintenance)
  - Solar charging inference from battery voltage trend
  - Deep sleep every 5 mins during battery-only condition (loads safely off)
  - Dummy data switches for dry test (uncalibrated sensors)

  ---------------------------------------------------------------
  REQUIRED LIBRARIES (Arduino Library Manager):
  ---------------------------------------------------------------
  - Firebase ESP Client by mobizt
  - LiquidCrystal I2C
  - DallasTemperature
  - OneWire
  - RTClib
  - FastLED
  - ESP32Servo

  ---------------------------------------------------------------
  SECURITY NOTE:
  ---------------------------------------------------------------
  Credentials below are placeholders only. Fill your own values.
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <RTClib.h>
#include <FastLED.h>
#include <ESP32Servo.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// ---------------------------------------------------------------
// ESP32 Arduino Core 3.x compatibility
// Core 2.x used ADC_11db; Core 3.x renamed it ADC_ATTEN_DB_12.
// ---------------------------------------------------------------
#ifndef ADC_11db
  #define ADC_11db ADC_ATTEN_DB_12
#endif

// =======================================================
// 1) CENTRALIZED PIN CONFIGURATION (STRICT)
// =======================================================
#define PH_SENSOR_PIN            34
#define TDS_SENSOR_PIN           35
#define TURBIDITY_SENSOR_PIN     32
#define TEMP_SENSOR_PIN          25
#define BMS_BATTERY_PIN          33

#define FAN_RELAY_PIN            26
#define PUMP_RELAY_PIN           27
#define HEATER_RELAY_PIN         17
#define FEEDER_SERVO_PIN         23
#define ARGB_DATA_PIN            16
#define BUZZER_PIN               13
#define WIFI_STATUS_LED_PIN      4

#define I2C_SDA_PIN              21
#define I2C_SCL_PIN              22

// =======================================================
// 2) PRIVACY & SECURITY (PLACEHOLDERS ONLY)
// =======================================================
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

#define API_KEY       "YOUR_FIREBASE_API_KEY"
#define DATABASE_URL  "YOUR_FIREBASE_DATABASE_URL"
#define USER_EMAIL    "YOUR_FIREBASE_USER_EMAIL"
#define USER_PASSWORD "YOUR_FIREBASE_USER_PASSWORD"

#define DB_ROOT "/devices/aquaSense"

// =======================================================
// System / App Config
// =======================================================
#define LCD_ADDR   0x27
#define LCD_COLS   16
#define LCD_ROWS   2

#define ARGB_COUNT 12
#define ARGB_TYPE  WS2812B
#define ARGB_ORDER GRB

// Timing (non-blocking)
const unsigned long SENSOR_INTERVAL_MS      = 2000;
const unsigned long CONTROL_INTERVAL_MS     = 1000;
const unsigned long LCD_INTERVAL_MS         = 2000;
const unsigned long FIREBASE_PUSH_MS        = 5000;
const unsigned long WIFI_RETRY_MS           = 10000;
const unsigned long FIREBASE_RETRY_MS       = 12000;
const unsigned long BATT_TREND_SAMPLE_MS    = 60000;
const unsigned long NIGHT_BREATH_STEP_MS    = 40;

// Deep sleep
const uint64_t DEEP_SLEEP_5MIN_US = 5ULL * 60ULL * 1000000ULL;

// Battery divider correction (MANDATORY)
const float BATTERY_DIVIDER_RATIO = 4.0f; // ADC_V * 4.0 exactly

// Battery / AC inference thresholds (tune as needed)
const float BATTERY_ONLY_VOLT_THRESHOLD = 11.2f; // inferred battery-only
const uint8_t BATTERY_ONLY_REQUIRED_SAMPLES = 3;

// Thresholds (tune/calibrate)
float TEMP_LOW_C               = 25.0f;
float TEMP_HIGH_C              = 29.0f;
float PH_LOW_LIMIT             = 6.3f;
float PH_HIGH_LIMIT            = 8.3f;
float TURBIDITY_SAFE_NTU       = 35.0f;
float TURBIDITY_CRITICAL_NTU   = 70.0f;
float TDS_SAFE_PPM             = 500.0f;
float TDS_CRITICAL_MAX_PPM     = 1200.0f;

// Filter alert: 2h continuous pump because turbidity not improving
const unsigned long FILTER_ALERT_WINDOW_MS = 2UL * 60UL * 60UL * 1000UL;

// Auto water-change safeguard: max 8 min
const unsigned long WATER_CHANGE_MAX_MS = 8UL * 60UL * 1000UL;
const unsigned long WATER_CHANGE_COOLDOWN_MS = 60UL * 60UL * 1000UL;

// 240V Heater safety watchdog (prevents infinite ON if sensor/element faults)
const unsigned long HEATER_MAX_ON_MS        = 30UL * 60UL * 1000UL; // 30 min hard cut-off
const float         TEMP_SENSOR_MIN_VALID_C = 1.0f;   // below this → sensor open/fault
const float         TEMP_SENSOR_MAX_VALID_C = 40.0f;  // above this → sensor fault / DS18B20 sentinel

// Buzzer pattern
const unsigned long BUZZER_TOGGLE_MS = 400;

// pH placeholder calibration (must calibrate in real system)
float PH_SLOPE  = -5.70f;
float PH_OFFSET = 21.34f;

// =======================================================
// Dummy Data Mode (for uncalibrated sensors / dry-test)
// =======================================================
bool DUMMY_PH         = false;
bool DUMMY_TDS        = false;
bool DUMMY_TURBIDITY  = false;
bool DUMMY_TEMP       = false;
bool DUMMY_BATTERY    = false;

// =======================================================
// Objects
// =======================================================
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);
RTC_DS3231 rtc;
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature ds18b20(&oneWire);
CRGB leds[ARGB_COUNT];
Servo feederServo;
WebServer server(80);

// Firebase
FirebaseData fbdo;
FirebaseData fbStream;
FirebaseAuth auth;
FirebaseConfig config;

// =======================================================
// Runtime State
// =======================================================
struct SensorState {
  float phValue = 7.0f;
  float phVoltage = 0.0f;
  float tdsPPM = 0.0f;
  float tdsVoltage = 0.0f;
  float turbidityNTU = 0.0f;
  float turbidityVoltage = 0.0f;
  float tempC = 26.0f;
  float battVoltage = 12.0f;
  float battPercent = 80.0f;
};

struct ControlState {
  bool manualMode = false;
  bool manualFan = false;
  bool manualPump = false;
  bool manualHeater = false;
  bool manualBuzzer = false;
  bool feedNow = false;
  bool voiceFeed = false;
  bool voicePump = false;
};

struct OutputState {
  bool fan = false;
  bool pump = false;
  bool heater = false;
  bool buzzer = false;
};

SensorState gS;
ControlState gC;
OutputState gO;

bool wifiConnected = false;
bool firebaseReady = false;
bool filterChangeRequired = false;
bool waterChangeActive = false;
bool chargingInferred = false;
bool batteryOnlyMode = false;

// timers
unsigned long tSensor = 0;
unsigned long tControl = 0;
unsigned long tLCD = 0;
unsigned long tFirebasePush = 0;
unsigned long tWiFiRetry = 0;
unsigned long tFirebaseRetry = 0;
unsigned long tBattTrend = 0;
unsigned long tNightBreath = 0;
unsigned long tBuzzer = 0;

// feeder FSM
enum FeedState { FEED_IDLE, FEED_TO_ANGLE, FEED_HOLD, FEED_RETURN };
FeedState feedState = FEED_IDLE;
unsigned long tFeed = 0;

// filter runtime tracking
unsigned long turbidityPumpStartMs = 0;

// water change tracking
unsigned long waterChangeStartMs = 0;
unsigned long lastWaterChangeStopMs = 0;

// 240V heater watchdog
unsigned long heaterOnStartMs   = 0;
bool          heaterSafetyCutoff = false;

// LCD pages
uint8_t lcdPage = 0;

// night breathing
uint8_t breathV = 20;
int8_t breathDir = 1;

// turbidity SMA + trend
const size_t TURB_SMA_N = 20;
float turbSMAArr[TURB_SMA_N] = {0};
size_t turbSMAIdx = 0;
size_t turbSMACount = 0;
float turbSMA = 0.0f;

const size_t TURB_TREND_N = 12; // 12 points x 1 min ~ 12 min trend
float turbTrendVal[TURB_TREND_N] = {0};
unsigned long turbTrendTime[TURB_TREND_N] = {0};
size_t turbTrendIdx = 0;
size_t turbTrendCount = 0;
float predictedDaysToMaintenance = -1.0f;

// battery trend for charging inference
const size_t BATT_TREND_N = 10;
float battTrend[BATT_TREND_N] = {0};
size_t battTrendIdx = 0;
size_t battTrendCount = 0;
uint8_t batteryOnlySamples = 0;

// =======================================================
// Helpers
// =======================================================
float clampf(float v, float lo, float hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

float randf(float a, float b) {
  return a + (float)(esp_random() % 10001) / 10000.0f * (b - a);
}

float adcToVoltage(int raw) {
  return (3.3f * (float)raw) / 4095.0f;
}

String rtcTimestamp() {
  DateTime n = rtc.now();
  char b[25];
  snprintf(b, sizeof(b), "%04d-%02d-%02dT%02d:%02d:%02d",
           n.year(), n.month(), n.day(), n.hour(), n.minute(), n.second());
  return String(b);
}

void allLoadsSafeOff() {
  digitalWrite(FAN_RELAY_PIN, LOW);
  digitalWrite(PUMP_RELAY_PIN, LOW);
  digitalWrite(HEATER_RELAY_PIN, LOW);
  gO.fan = gO.pump = gO.heater = false;
}

void applyOutputs() {
  digitalWrite(FAN_RELAY_PIN,    gO.fan    ? HIGH : LOW);
  digitalWrite(PUMP_RELAY_PIN,   gO.pump   ? HIGH : LOW);
  digitalWrite(HEATER_RELAY_PIN, gO.heater ? HIGH : LOW);
  // NOTE: Buzzer is managed exclusively by the non-blocking 400 ms toggle in loop().
  // Writing BUZZER_PIN here would fight the toggle every 1 s and corrupt the beep pattern.
}

void updateStatusLed() {
  // ON when both Wi-Fi and Firebase are ready, else OFF
  digitalWrite(WIFI_STATUS_LED_PIN, (wifiConnected && firebaseReady) ? HIGH : LOW);
}

// =======================================================
// Sensor Reading
// =======================================================
float readPH() {
  if (DUMMY_PH) {
    gS.phVoltage = 2.5f;
    return randf(6.8f, 7.8f);
  }
  int raw = analogRead(PH_SENSOR_PIN);
  gS.phVoltage = adcToVoltage(raw);
  float ph = PH_OFFSET + PH_SLOPE * gS.phVoltage;
  return clampf(ph, 0.0f, 14.0f);
}

float readTempC() {
  if (DUMMY_TEMP) return randf(24.0f, 30.5f);
  ds18b20.requestTemperatures();
  float t = ds18b20.getTempCByIndex(0);
  // Reject: out-of-range OR DS18B20 power-on sentinel (85.0 °C returned before first valid conversion)
  if (t < -55.0f || t > 125.0f || fabsf(t - 85.0f) < 0.1f) return gS.tempC;
  return t;
}

float readTurbidity() {
  if (DUMMY_TURBIDITY) {
    gS.turbidityVoltage = 2.0f;
    return randf(15.0f, 80.0f);
  }
  int raw = analogRead(TURBIDITY_SENSOR_PIN);
  gS.turbidityVoltage = adcToVoltage(raw);
  float ntu = -1120.4f * gS.turbidityVoltage * gS.turbidityVoltage
            + 5742.3f * gS.turbidityVoltage - 4352.9f;
  if (isnan(ntu) || ntu < 0) ntu = 0;
  return ntu;
}

float readTDS(float tempC) {
  if (DUMMY_TDS) {
    gS.tdsVoltage = 2.1f;
    return randf(250.0f, 1100.0f);
  }
  int raw = analogRead(TDS_SENSOR_PIN);
  gS.tdsVoltage = adcToVoltage(raw);
  float cc = 1.0f + 0.02f * (tempC - 25.0f);
  float cv = gS.tdsVoltage / cc;
  float tds = (133.42f * cv * cv * cv - 255.86f * cv * cv + 857.39f * cv) * 0.5f;
  if (isnan(tds) || tds < 0) tds = 0;
  return tds;
}

float readBatteryV() {
  if (DUMMY_BATTERY) return randf(10.8f, 12.8f);
  int raw = analogRead(BMS_BATTERY_PIN);
  float vAdc = adcToVoltage(raw);
  return vAdc * BATTERY_DIVIDER_RATIO; // exact requirement
}

void updateTurbiditySMA(float x) {
  turbSMAArr[turbSMAIdx] = x;
  turbSMAIdx = (turbSMAIdx + 1) % TURB_SMA_N;
  if (turbSMACount < TURB_SMA_N) turbSMACount++;

  float sum = 0;
  for (size_t i = 0; i < turbSMACount; i++) sum += turbSMAArr[i];
  turbSMA = sum / (float)turbSMACount;
}

void updateTurbidityTrendPrediction(float x) {
  turbTrendVal[turbTrendIdx] = x;
  turbTrendTime[turbTrendIdx] = millis();
  turbTrendIdx = (turbTrendIdx + 1) % TURB_TREND_N;
  if (turbTrendCount < TURB_TREND_N) turbTrendCount++;

  if (turbTrendCount < 4) {
    predictedDaysToMaintenance = -1.0f;
    return;
  }

  size_t oldest = (turbTrendIdx + TURB_TREND_N - turbTrendCount) % TURB_TREND_N;
  size_t newest = (turbTrendIdx + TURB_TREND_N - 1) % TURB_TREND_N;

  float y0 = turbTrendVal[oldest];
  float y1 = turbTrendVal[newest];
  unsigned long dt = turbTrendTime[newest] - turbTrendTime[oldest];
  if (dt == 0) return;

  float slope = (y1 - y0) / (float)dt; // NTU per ms
  if (slope <= 0) {
    predictedDaysToMaintenance = 999.0f; // improving/stable
    return;
  }

  float remain = TURBIDITY_CRITICAL_NTU - y1;
  if (remain <= 0) {
    predictedDaysToMaintenance = 0.0f;
    return;
  }

  float msToCritical = remain / slope;
  predictedDaysToMaintenance = msToCritical / (1000.0f * 60.0f * 60.0f * 24.0f);
  if (predictedDaysToMaintenance > 999.0f) predictedDaysToMaintenance = 999.0f;
}

void updateChargingInference(float battV) {
  battTrend[battTrendIdx] = battV;
  battTrendIdx = (battTrendIdx + 1) % BATT_TREND_N;
  if (battTrendCount < BATT_TREND_N) battTrendCount++;

  if (battTrendCount < 4) return;

  size_t oldest = (battTrendIdx + BATT_TREND_N - battTrendCount) % BATT_TREND_N;
  size_t newest = (battTrendIdx + BATT_TREND_N - 1) % BATT_TREND_N;
  float delta = battTrend[newest] - battTrend[oldest];

  chargingInferred = (delta > 0.05f); // >50mV trend considered charging

  if (battV < BATTERY_ONLY_VOLT_THRESHOLD && !chargingInferred) {
    if (batteryOnlySamples < 255) batteryOnlySamples++;
  } else {
    batteryOnlySamples = 0;
  }
  batteryOnlyMode = (batteryOnlySamples >= BATTERY_ONLY_REQUIRED_SAMPLES);
}

void readAllSensors() {
  gS.tempC = readTempC();
  gS.phValue = readPH();
  gS.turbidityNTU = readTurbidity();
  gS.tdsPPM = readTDS(gS.tempC);
  gS.battVoltage = readBatteryV();

  // Battery % for a 3S Li-ion style pack default range
  gS.battPercent = clampf((gS.battVoltage - 9.0f) * 100.0f / (12.6f - 9.0f), 0.0f, 100.0f);

  updateTurbiditySMA(gS.turbidityNTU);
}

// =======================================================
// ARGB Lighting by RTC (no NTP)
// =======================================================
void setSolidColor(const CRGB &c, uint8_t brightness) {
  FastLED.setBrightness(brightness);
  fill_solid(leds, ARGB_COUNT, c);
  FastLED.show();
}

void setNightBreathing() {
  if (millis() - tNightBreath < NIGHT_BREATH_STEP_MS) return;
  tNightBreath = millis();

  breathV = (uint8_t)((int)breathV + breathDir * 2);
  if (breathV >= 140) breathDir = -1;
  if (breathV <= 15) breathDir = 1;

  FastLED.setBrightness(70);
  for (int i = 0; i < ARGB_COUNT; i++) {
    leds[i] = (i % 2 == 0) ? CHSV(170, 170, breathV) : CHSV(192, 150, breathV);
  }
  FastLED.show();
}

void updateLightingByRTC() {
  DateTime now = rtc.now();
  int h = now.hour();

  if (h >= 6 && h < 10) {
    // Morning sunrise: dim orange/yellow
    setSolidColor(CRGB(255, 145, 40), 90);
  } else if (h >= 10 && h < 18) {
    // Day: bright white/light blue
    setSolidColor(CRGB(180, 225, 255), 180);
  } else {
    // Night: calm breathing blue/purple
    setNightBreathing();
  }
}

// =======================================================
// Feeder Servo FSM
// =======================================================
void requestFeed() {
  if (feedState == FEED_IDLE) gC.feedNow = true;
}

void updateFeeder() {
  switch (feedState) {
    case FEED_IDLE:
      if (gC.feedNow) {
        feederServo.write(95);
        tFeed = millis();
        feedState = FEED_TO_ANGLE;
        gC.feedNow = false;
      }
      break;
    case FEED_TO_ANGLE:
      if (millis() - tFeed >= 600) {
        tFeed = millis();
        feedState = FEED_HOLD;
      }
      break;
    case FEED_HOLD:
      if (millis() - tFeed >= 800) {
        feederServo.write(15);
        tFeed = millis();
        feedState = FEED_RETURN;
      }
      break;
    case FEED_RETURN:
      if (millis() - tFeed >= 500) {
        feedState = FEED_IDLE;
      }
      break;
  }
}

// =======================================================
// Control & Automation
// =======================================================
void startWaterChange() {
  if (waterChangeActive) return;
  if (millis() - lastWaterChangeStopMs < WATER_CHANGE_COOLDOWN_MS) return;
  waterChangeActive = true;
  waterChangeStartMs = millis();
}

void stopWaterChange() {
  waterChangeActive = false;
  lastWaterChangeStopMs = millis();
}

void evaluateFilterAlertLogic() {
  // condition: pump ON due to high turbidity in AUTO mode
  if (!gC.manualMode && gO.pump && gS.turbidityNTU > TURBIDITY_SAFE_NTU) {
    if (turbidityPumpStartMs == 0) {
      turbidityPumpStartMs = millis();
    } else {
      unsigned long runMs = millis() - turbidityPumpStartMs;
      if (runMs >= FILTER_ALERT_WINDOW_MS && gS.turbidityNTU > TURBIDITY_SAFE_NTU) {
        filterChangeRequired = true;
      }
    }
  } else {
    turbidityPumpStartMs = 0;
  }
}

void evaluateAutomation() {
  // Voice hooks (from Firebase commands)
  if (gC.voiceFeed) {
    requestFeed();
    gC.voiceFeed = false;
  }
  if (gC.voicePump) {
    gO.pump = true;
    gC.voicePump = false;
  }

  // Auto water change trigger
  if (!gC.manualMode && gS.tdsPPM >= TDS_CRITICAL_MAX_PPM) {
    startWaterChange();
  }

  if (gC.manualMode) {
    gO.fan    = gC.manualFan;
    gO.pump   = gC.manualPump;
    gO.heater = gC.manualHeater;
    gO.buzzer = gC.manualBuzzer;
    // Operator owns the heater in manual mode — clear the safety watchdog
    heaterOnStartMs    = 0;
    heaterSafetyCutoff = false;
  } else {
    // ── Fan & pump (simple threshold) ──────────────────────────────────────
    gO.fan  = (gS.tempC > TEMP_HIGH_C);
    gO.pump = (gS.turbidityNTU > TURBIDITY_SAFE_NTU) || (gS.tdsPPM > TDS_SAFE_PPM);

    // ── Alarms ─────────────────────────────────────────────────────────────
    gO.buzzer = (gS.phValue < PH_LOW_LIMIT  || gS.phValue > PH_HIGH_LIMIT  ||
                 gS.turbidityNTU > TURBIDITY_CRITICAL_NTU                    ||
                 gS.tdsPPM       > TDS_CRITICAL_MAX_PPM);

    // ── 240 V Heater: safety-guarded ───────────────────────────────────────
    // Guard 1: sensor validity (rejects DS18B20 faults, open-circuit, OOR values)
    bool tempValid = (gS.tempC >= TEMP_SENSOR_MIN_VALID_C &&
                      gS.tempC <= TEMP_SENSOR_MAX_VALID_C);
    if (!tempValid) {
      gO.heater          = false;   // unknown water temp → never energise 240 V element
      heaterOnStartMs    = 0;
      heaterSafetyCutoff = false;
      gO.buzzer          = true;    // alert: temp sensor fault
    } else {
      bool wantHeater = (gS.tempC < TEMP_LOW_C);

      if (wantHeater && !heaterSafetyCutoff) {
        if (!gO.heater) heaterOnStartMs = millis(); // capture rising edge
        gO.heater = true;
        // Guard 2: hard cut-off after HEATER_MAX_ON_MS (30 min)
        if (millis() - heaterOnStartMs >= HEATER_MAX_ON_MS) {
          heaterSafetyCutoff = true;
          gO.heater          = false;
          gO.buzzer          = true;  // alert: heater timeout — check element / water temp
        }
      } else if (!wantHeater) {
        // Temp recovered → fully reset watchdog so next cold cycle starts fresh
        gO.heater          = false;
        heaterOnStartMs    = 0;
        heaterSafetyCutoff = false;
      } else {
        // Safety cutoff active and temp still cold — keep heater OFF until manually reset
        gO.heater = false;
      }
    }
  }

  // Water change override + safeguard
  if (waterChangeActive) {
    gO.pump = true;
    if (millis() - waterChangeStartMs >= WATER_CHANGE_MAX_MS || gS.tdsPPM < TDS_SAFE_PPM) {
      stopWaterChange();
    }
  }

  evaluateFilterAlertLogic();
  applyOutputs();
}

// =======================================================
// Firebase
// =======================================================
void streamTimeoutCallback(bool timeout) {
  if (timeout) Serial.println("[Firebase] Stream timeout");
  if (!fbStream.httpConnected()) {
    Serial.print("[Firebase] Stream disconnected: ");
    Serial.println(fbStream.errorReason());
  }
}

void parseControlJson(FirebaseJson *json) {
  FirebaseJsonData d;
  if (json->get(d, "manual_mode")) gC.manualMode = d.boolValue;
  if (json->get(d, "manual_fan")) gC.manualFan = d.boolValue;
  if (json->get(d, "manual_pump")) gC.manualPump = d.boolValue;
  if (json->get(d, "manual_heater")) gC.manualHeater = d.boolValue;
  if (json->get(d, "manual_buzzer")) gC.manualBuzzer = d.boolValue;
  if (json->get(d, "feed_now") && d.boolValue) requestFeed();
}

void parseCommandsJson(FirebaseJson *json) {
  FirebaseJsonData d;
  if (json->get(d, "voice_feed") && d.boolValue) gC.voiceFeed = true;
  if (json->get(d, "voice_pump") && d.boolValue) gC.voicePump = true;
}

void streamCallback(FirebaseStream data) {
  String root = data.streamPath(); // "/controls" or "/commands"
  String path = data.dataPath();
  String type = data.dataType();

  if (root == "/controls") {
    if (path == "/" && type == "json") {
      parseControlJson(data.jsonObjectPtr());
    } else if (type == "boolean") {
      bool b = data.boolData();
      if (path == "/manual_mode") gC.manualMode = b;
      else if (path == "/manual_fan") gC.manualFan = b;
      else if (path == "/manual_pump") gC.manualPump = b;
      else if (path == "/manual_heater") gC.manualHeater = b;
      else if (path == "/manual_buzzer") gC.manualBuzzer = b;
      else if (path == "/feed_now" && b) requestFeed();
    }
  }

  if (root == "/commands") {
    if (path == "/" && type == "json") {
      parseCommandsJson(data.jsonObjectPtr());
    } else if (type == "boolean") {
      bool b = data.boolData();
      if (path == "/voice_feed" && b) gC.voiceFeed = true;
      if (path == "/voice_pump" && b) gC.voicePump = true;
    }
  }
}

void initFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void tryFirebaseStart() {
  if (!wifiConnected || firebaseReady) return;

  initFirebase();
  unsigned long st = millis();
  while (millis() - st < 2500) {
    if (Firebase.ready()) break;
    yield();
  }

  firebaseReady = Firebase.ready();
  if (!firebaseReady) return;

  // stream controls
  if (!Firebase.RTDB.beginStream(&fbStream, String(DB_ROOT) + "/controls")) {
    Serial.print("[Firebase] controls stream fail: ");
    Serial.println(fbStream.errorReason());
  } else {
    Firebase.RTDB.setStreamCallback(&fbStream, streamCallback, streamTimeoutCallback);
  }

  // ensure voice hook structure exists
  FirebaseJson cmdInit;
  cmdInit.set("voice_feed", false);
  cmdInit.set("voice_pump", false);
  Firebase.RTDB.updateNodeAsync(&fbdo, String(DB_ROOT) + "/commands", &cmdInit);
}

void pushFirebase() {
  if (!firebaseReady || !Firebase.ready()) return;

  FirebaseJson sensors;
  sensors.set("timestamp", rtcTimestamp());
  sensors.set("temperature_c", gS.tempC);
  sensors.set("ph_value", gS.phValue);
  sensors.set("ph_voltage", gS.phVoltage);
  sensors.set("tds_ppm", gS.tdsPPM);
  sensors.set("tds_voltage", gS.tdsVoltage);
  sensors.set("turbidity_ntu", gS.turbidityNTU);
  sensors.set("turbidity_sma", turbSMA);
  sensors.set("predicted_days_to_maintenance", predictedDaysToMaintenance);
  sensors.set("battery_voltage", gS.battVoltage);
  sensors.set("battery_percent", gS.battPercent);
  sensors.set("charging_inferred", chargingInferred);
  sensors.set("dummy_ph", DUMMY_PH);
  sensors.set("dummy_tds", DUMMY_TDS);
  sensors.set("dummy_turbidity", DUMMY_TURBIDITY);
  sensors.set("dummy_temp", DUMMY_TEMP);
  sensors.set("dummy_battery", DUMMY_BATTERY);
  Firebase.RTDB.updateNodeAsync(&fbdo, String(DB_ROOT) + "/sensors", &sensors);

  FirebaseJson status;
  status.set("manual_mode", gC.manualMode);
  status.set("fan", gO.fan);
  status.set("pump", gO.pump);
  status.set("heater", gO.heater);
  status.set("buzzer", gO.buzzer);
  status.set("water_change_active", waterChangeActive);
  status.set("filter_change_required", filterChangeRequired);
  status.set("wifi_connected", wifiConnected);
  status.set("firebase_ready", firebaseReady);
  status.set("battery_only_mode", batteryOnlyMode);
  Firebase.RTDB.updateNodeAsync(&fbdo, String(DB_ROOT) + "/status", &status);

  if (filterChangeRequired) {
    Firebase.RTDB.setStringAsync(&fbdo, String(DB_ROOT) + "/alerts/filter_change_required_at", rtcTimestamp());
  }

  // reset command hooks (one-shot friendly for IFTTT)
  Firebase.RTDB.setBoolAsync(&fbdo, String(DB_ROOT) + "/commands/voice_feed", false);
  Firebase.RTDB.setBoolAsync(&fbdo, String(DB_ROOT) + "/commands/voice_pump", false);
}

// =======================================================
// Wi-Fi
// =======================================================
void tryWiFiConnect() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void monitorWiFiState() {
  wifiConnected = (WiFi.status() == WL_CONNECTED);
  if (!wifiConnected) firebaseReady = false;
}

// =======================================================
// Local Web Dashboard
// =======================================================
String dashboardHTML() {
  String html;
  html.reserve(4500);
  html += "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>Smart Aquarium Dashboard</title>";
  html += "<style>body{font-family:Arial;background:#0b1220;color:#e8efff;padding:18px}";
  html += ".card{background:#121b2f;border-radius:12px;padding:14px;margin:10px 0}";
  html += "button{padding:10px 14px;border:0;border-radius:8px;background:#2d7cff;color:#fff;font-weight:bold}";
  html += "table{width:100%}td{padding:6px 4px;border-bottom:1px solid #1d2a42}</style></head><body>";
  html += "<h2>Smart Aquarium Monitor & Control</h2>";
  html += "<div class='card'><b>Time:</b> " + rtcTimestamp() + "<br>";
  html += "<b>Mode:</b> " + String(gC.manualMode ? "MANUAL" : "AUTO");
  html += " | <b>Wi-Fi:</b> " + String(wifiConnected ? "Connected" : "Offline");
  html += " | <b>Firebase:</b> " + String(firebaseReady ? "Ready" : "Offline") + "</div>";

  html += "<div class='card'><table>";
  html += "<tr><td>Temperature</td><td>" + String(gS.tempC, 2) + " C</td></tr>";
  html += "<tr><td>pH</td><td>" + String(gS.phValue, 2) + "</td></tr>";
  html += "<tr><td>TDS</td><td>" + String(gS.tdsPPM, 1) + " ppm</td></tr>";
  html += "<tr><td>Turbidity</td><td>" + String(gS.turbidityNTU, 1) + " NTU</td></tr>";
  html += "<tr><td>Turbidity SMA</td><td>" + String(turbSMA, 1) + " NTU</td></tr>";
  html += "<tr><td>Predicted Maintenance</td><td>" + String(predictedDaysToMaintenance, 1) + " days</td></tr>";
  html += "<tr><td>Battery</td><td>" + String(gS.battVoltage, 2) + " V (" + String(gS.battPercent, 0) + "%)</td></tr>";
  html += "<tr><td>Charging Inferred</td><td>" + String(chargingInferred ? "YES" : "NO") + "</td></tr>";
  html += "<tr><td>Fan/Pump/Heater</td><td>" + String(gO.fan) + "/" + String(gO.pump) + "/" + String(gO.heater) + "</td></tr>";
  html += "</table></div>";

  html += "<div class='card'><form action='/feed' method='POST'><button type='submit'>Trigger Fish Feeder</button></form></div>";
  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", dashboardHTML());
}

void handleFeed() {
  requestFeed();
  server.send(200, "text/plain", "Feeding triggered");
}

void startWebServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/feed", HTTP_POST, handleFeed);
  server.begin();
}

// =======================================================
// LCD
// =======================================================
void updateLCD() {
  lcd.clear();
  if (lcdPage == 0) {
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(gS.tempC, 1);
    lcd.print("C pH:");
    lcd.print(gS.phValue, 1);

    lcd.setCursor(0, 1);
    lcd.print("TDS:");
    lcd.print(gS.tdsPPM, 0);
    lcd.print("ppm");
  } else if (lcdPage == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Turb:");
    lcd.print(gS.turbidityNTU, 0);
    lcd.print("NTU");

    lcd.setCursor(0, 1);
    lcd.print("Batt:");
    lcd.print(gS.battVoltage, 1);
    lcd.print("V ");
    lcd.print(gS.battPercent, 0);
    lcd.print("%");
  } else {
    lcd.setCursor(0, 0);
    lcd.print(gC.manualMode ? "MODE:MANUAL" : "MODE:AUTO");

    lcd.setCursor(0, 1);
    if (filterChangeRequired) lcd.print("FILTER CHANGE!");
    else if (waterChangeActive) lcd.print("WATER CHANGE...");
    else lcd.print(wifiConnected ? "WiFi OK" : "Offline OK");
  }

  lcdPage = (lcdPage + 1) % 3;
}

// =======================================================
// Deep Sleep
// =======================================================
void enterDeepSleepCycle() {
  allLoadsSafeOff();
  digitalWrite(BUZZER_PIN, LOW);
  FastLED.clear(true);
  feederServo.detach();

  WiFi.mode(WIFI_OFF);
  btStop();

  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_5MIN_US);
  esp_deep_sleep_start();
}

// =======================================================
// Setup
// =======================================================
void setup() {
  Serial.begin(115200);

  // Pin modes
  pinMode(FAN_RELAY_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(HEATER_RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(WIFI_STATUS_LED_PIN, OUTPUT);

  allLoadsSafeOff();
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(WIFI_STATUS_LED_PIN, LOW);

  // ADC settings
  analogReadResolution(12);
  analogSetPinAttenuation(PH_SENSOR_PIN, ADC_11db);
  analogSetPinAttenuation(TDS_SENSOR_PIN, ADC_11db);
  analogSetPinAttenuation(TURBIDITY_SENSOR_PIN, ADC_11db);
  analogSetPinAttenuation(BMS_BATTERY_PIN, ADC_11db);

  // I2C + RTC + LCD
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("Aquarium Booting");
  lcd.setCursor(0, 1); lcd.print("RTC/LCD Init...");

  if (!rtc.begin()) {
    lcd.clear();
    lcd.print("RTC NOT FOUND!");
    Serial.println("[RTC] DS3231 not detected");
  } else {
    if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // fallback only
    }
  }

  // DS18B20
  ds18b20.begin();

  // ARGB
  FastLED.addLeds<ARGB_TYPE, ARGB_DATA_PIN, ARGB_ORDER>(leds, ARGB_COUNT);
  FastLED.clear(true);

  // Servo
  feederServo.setPeriodHertz(50);
  feederServo.attach(FEEDER_SERVO_PIN, 500, 2400);
  feederServo.write(15);

  // Wi-Fi
  WiFi.mode(WIFI_STA);
  tryWiFiConnect();

  // Web
  startWebServer();

  // initial sample
  readAllSensors();
  updateTurbidityTrendPrediction(gS.turbidityNTU);
  updateChargingInference(gS.battVoltage);
  updateLightingByRTC();

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("System Ready");
  lcd.setCursor(0, 1); lcd.print("Offline-safe AUTO");

  unsigned long now = millis();
  tSensor = tControl = tLCD = tFirebasePush = tWiFiRetry = tFirebaseRetry = tBattTrend = now;
}

// =======================================================
// Main Loop (NON-BLOCKING)
// =======================================================
void loop() {
  unsigned long now = millis();

  // local dashboard
  server.handleClient();

  // Wi-Fi monitor + retry
  monitorWiFiState();
  if (!wifiConnected && now - tWiFiRetry >= WIFI_RETRY_MS) {
    tWiFiRetry = now;
    tryWiFiConnect();
  }

  // Firebase retry
  if (wifiConnected && !firebaseReady && now - tFirebaseRetry >= FIREBASE_RETRY_MS) {
    tFirebaseRetry = now;
    tryFirebaseStart();
  }

  // sensor cycle
  if (now - tSensor >= SENSOR_INTERVAL_MS) {
    tSensor = now;
    readAllSensors();
  }

  // control/automation cycle
  if (now - tControl >= CONTROL_INTERVAL_MS) {
    tControl = now;
    evaluateAutomation();
  }

  // feeder FSM
  updateFeeder();

  // non-blocking buzzer in normal mode (if ON)
  if (gO.buzzer) {
    if (now - tBuzzer >= BUZZER_TOGGLE_MS) {
      tBuzzer = now;
      digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // RTC-based ARGB
  updateLightingByRTC();

  // LCD cycle
  if (now - tLCD >= LCD_INTERVAL_MS) {
    tLCD = now;
    updateLCD();
  }

  // battery trend inference + turbidity trend (every minute)
  if (now - tBattTrend >= BATT_TREND_SAMPLE_MS) {
    tBattTrend = now;
    updateChargingInference(gS.battVoltage);
    updateTurbidityTrendPrediction(gS.turbidityNTU);
  }

  // Firebase push
  if (now - tFirebasePush >= FIREBASE_PUSH_MS) {
    tFirebasePush = now;
    pushFirebase();
  }

  // connection LED
  updateStatusLed();

  // If firebase dropped
  if (firebaseReady && !Firebase.ready()) {
    firebaseReady = false;
  }

  // Deep sleep when inferred battery-only mode
  // requirement: safely turn off high-drain loads and sleep 5 min cycle
  if (batteryOnlyMode) {
    // quick best-effort push before sleep if cloud available
    if (wifiConnected && firebaseReady && Firebase.ready()) {
      pushFirebase();
    }
    enterDeepSleepCycle();
  }
}