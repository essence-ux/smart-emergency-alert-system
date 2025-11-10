/ Your Blynk credentials
// Get these from the Blynk console
#define BLYNK_TEMPLATE_ID "TMPL38PRzStkq"
#define BLYNK_TEMPLATE_NAME "ALERT"
#define BLYNK_AUTH_TOKEN "jKsKfMJDVBzck6Q1DO5bRs38WWP7vaIT"
#include <BlynkSimpleEsp32.h>
#include <HardwareSerial.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Your WiFi credentials
const char* ssid = "Shadow";
const char* password = "genm7544";

// Serial ports
HardwareSerial gps(2); // GPS

// Pin definitions
#define GAS_PIN 34
#define FLAME_PIN 25
#define BUTTON_PIN 32

// Flags
bool alertTriggered = false;

// Timers for non-blocking code
unsigned long previousMillis = 0;
const long interval = 1000; // Interval to read sensors (in milliseconds)
BLYNK_CONNECTED() {
  Serial.println("✅ Connected to Blynk");
  Blynk.logEvent("network_connected", "Connection secure");
}

void setup() {
  Serial.begin(115200);

  // Sensor setup
  pinMode(GAS_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // GPS Serial (RX, TX)
  gps.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  // This must be called frequently to maintain the Blynk connection
  Blynk.run();

  // Use a non-blocking timer to read sensors and print to Serial
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    int gasValue = analogRead(GAS_PIN);
    int flameValue = digitalRead(FLAME_PIN);
    int buttonValue = digitalRead(BUTTON_PIN);

    Serial.print("Gas: "); Serial.print(gasValue);
    Serial.print(" | Flame: "); Serial.print(flameValue);
    Serial.print(" | Button: "); Serial.println(buttonValue);

    // Trigger conditions
    if (gasValue > 3250 ){
      if (!alertTriggered) {
        Serial.println("Trigger condition met. Attempting to send notification...");
        // Read GPS data
        String gpsData = readGPS();
        // Send a Blynk notification
        Blynk.logEvent("gas_detected",gpsData);
        Serial.println("Blynk notification sent!");
        alertTriggered = true; // prevent repeated logging
      }
    }
    if(flameValue == LOW ){
      if (!alertTriggered) {
        Serial.println("Trigger condition met. Attempting to send notification...");
        // Read GPS data
        String gpsData = readGPS();
        // Send a Blynk notification
        Blynk.logEvent("fire_detected",gpsData);
        Serial.println("Blynk notification sent!");
        alertTriggered = true; // prevent repeated logging
      }
    } 
    if(buttonValue == LOW){
      if (!alertTriggered) {
        Serial.println("Trigger condition met. Attempting to send notification...");
        // Read GPS data
        String gpsData = readGPS();
        // Send a Blynk notification
        Blynk.logEvent("manual_emergency",gpsData);
        Serial.println("Blynk notification sent!");
        alertTriggered = true; // prevent repeated logging
      }
    }  else {
      alertTriggered = false; // reset when conditions are safe
    }
  }
}

// --- Get GPS data ---
String readGPS() {
  String lat = "12.973052"; // fallback demo
  String lon = "80.044600"; // fallback demo

  unsigned long start = millis();
  while (millis() - start < 3000) { // wait 3s for GPS
    if (gps.available()) {
      String nmea = gps.readStringUntil('\n');
      if (nmea.startsWith("$GPGGA")) {
        // For simplicity we just return demo coords
        // (can parse NMEA properly using TinyGPS++)
        return "https://maps.google.com/?q=" + lat + "," + lon;
      }
    }
  }
  return "https://maps.google.com/?q=" + lat + "," + lon;
}
