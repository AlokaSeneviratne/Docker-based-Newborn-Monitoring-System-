#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

// Sensor libraries
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MPU6050.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <math.h>

// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- WiFi ----------------
const char* ssid     = "TP-Link_3C98";
const char* password = "a1l2o3k4a";

// Backend REST API URL
const char* backend_url = "https://sds-final-project-psenevir.rahtiapp.fi/api/data";

// --------------- Pins -----------------
#define SDA_PIN   4
#define SCL_PIN   5
#define ONE_WIRE_BUS 14

// DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensors(&oneWire);

// MPU6050
MPU6050 mpu;

// MAX30102
MAX30105 particleSensor;
uint32_t irBuffer[100];
uint32_t redBuffer[100];

float   lastTempC       = NAN;
int32_t lastHR          = -1;
int32_t lastSpO2        = -1;
bool    lastHRValid     = false;
bool    lastSpO2Valid   = false;
float   lastMotionLevel = 0.0f;

unsigned long lastUpdateMs = 0;
const unsigned long UPDATE_INTERVAL_MS = 1000;

// ------- Classification Functions -------
String classifyTemp(float t) {
  if (isnan(t)) return "No reading";
  if (t < 36.0) return "Low";
  if (t <= 37.5) return "Normal";
  if (t < 38.0) return "Mild fever";
  return "Fever";
}

String classifyMotion(float ml) {
  if (ml < 0.1) return "Still";
  if (ml < 0.4) return "Normal";
  if (ml < 0.8) return "High";
  return "Very strong";
}

// ------------- WiFi Connect ----------------
void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ------------- Sensor Update ----------------
void updateSensors() {
  // DS18B20 temperature
  tempSensors.requestTemperatures();
  lastTempC = tempSensors.getTempCByIndex(0);

  // MPU6050 motion
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  float axg = ax / 16384.0;
  float ayg = ay / 16384.0;
  float azg = az / 16384.0;
  lastMotionLevel = fabs(sqrt(axg*axg + ayg*ayg + azg*azg) - 1.0);

  // MAX30102 (HR + SpO2)
  for (int i = 0; i < 100; i++) {
    while (!particleSensor.available()) {
      particleSensor.check();
      yield();
    }
    irBuffer[i]  = particleSensor.getIR();
    redBuffer[i] = particleSensor.getRed();
    particleSensor.nextSample();
    yield();
  }

  int32_t spo2, hr;
  int8_t spo2Valid, hrValid;

  maxim_heart_rate_and_oxygen_saturation(
    irBuffer, 100, redBuffer,
    &spo2, &spo2Valid, &hr, &hrValid
  );

  lastHR        = hr;
  lastSpO2      = spo2;
  lastHRValid   = (hrValid != 0);
  lastSpO2Valid = (spo2Valid != 0);

  // OLED update
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("HR: "); display.print(lastHR);
  display.setCursor(0,12);
  display.print("SpO2: "); display.print(lastSpO2);
  display.setCursor(0,24);
  display.print("Temp: "); display.print(lastTempC);
  display.setCursor(0,36);
  display.print("Motion: "); display.print(lastMotionLevel);
  display.display();
}

// ------------- POST Data to Backend ----------------
void sendDataToBackend() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost! Reconnecting...");
    connectWiFi();
  }

  WiFiClientSecure client;
  client.setInsecure();  

  HTTPClient https;

  Serial.println("Sending POST to backend...");

  if (https.begin(client, backend_url)) {
    https.addHeader("Content-Type", "application/json");

    // Prepare classification strings
    String tempStatus  = classifyTemp(lastTempC);
    String motionStatus = classifyMotion(lastMotionLevel);

    // Build REQUIRED JSON for backend
    String json = "{";
    json += "\"id\":\"esp8266-1\",";
    json += "\"hr\":" + String(lastHR) + ",";
    json += "\"hrValid\":" + String(lastHRValid ? "true" : "false") + ",";
    json += "\"spo2\":" + String(lastSpO2) + ",";
    json += "\"spo2Valid\":" + String(lastSpO2Valid ? "true" : "false") + ",";
    json += "\"temp\":" + String(lastTempC, 2) + ",";
    json += "\"tempStatus\":\"" + tempStatus + "\",";
    json += "\"motion\":" + String(lastMotionLevel, 3) + ",";
    json += "\"motionStatus\":\"" + motionStatus + "\",";
    json += "\"timestamp\":" + String(millis());
    json += "}";

    int code = https.POST(json);

    Serial.println("Sent JSON:");
    Serial.println(json);

    Serial.print("POST response code: ");
    Serial.println(code);

    https.end();
  } else {
    Serial.println("HTTPS begin failed");
  }
}


// ------------------ SETUP -------------------
void setup() {
  Serial.begin(115200);
  delay(200);

  Wire.begin(SDA_PIN, SCL_PIN);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  pinMode(ONE_WIRE_BUS, INPUT_PULLUP);
  tempSensors.begin();
  mpu.initialize();

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not found!");
  } else {
    particleSensor.setup();
    particleSensor.setPulseAmplitudeIR(0x2F);
    particleSensor.setPulseAmplitudeRed(0x2F);
  }

  connectWiFi();
  updateSensors();
}

// ------------------ LOOP -------------------
void loop() {
  if (millis() - lastUpdateMs >= UPDATE_INTERVAL_MS) {
    updateSensors();
    sendDataToBackend();
    lastUpdateMs = millis();
  }
}
