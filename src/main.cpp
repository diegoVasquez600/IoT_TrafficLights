#include <Arduino.h>
#include <lights/TrafficLight.h>
#include <lights/PedestrianLight.h>
#include <devices/Button.h>
#include <devices/Buzzer.h>
#include <controllers/TrafficController.h>
#include <controllers/SerialReporter.h>
#include <network/WiFiService.h>
#include <iot/WebServerService.h>

// Pin definitions
/// Traffic light pins
#define LED_GREEN 25
#define LED_YELLOW 26
#define LED_RED 27

/// Pedestrian light pins
#define LED_PEDESTRIAN_RED 33
#define LED_PEDESTRIAN_GREEN 32
/// Pedestrian button pin
#define BUTTON_PEDESTRIAN 17

// Buzzer pin
#define BUZZER_PIN 16

// Serial communication baud rate
#define SERIAL_BAUD_RATE 9600

// WiFi credentials
#define WIFI_SSID "Sudo_BashBoss"
#define WIFI_PASSWORD "Sudo Su <Mr.Knox/>"

// Set to 1 to print button level changes over Serial.
#define DEBUG_BUTTON 1



TrafficLight trafficLight(LED_RED, LED_YELLOW, LED_GREEN);
PedestrianLight pedestrianLight(LED_PEDESTRIAN_GREEN, LED_PEDESTRIAN_RED);
Button pedestrianButton(BUTTON_PEDESTRIAN);
Buzzer buzzer(BUZZER_PIN);
SerialReporter serialReporter;

TrafficController trafficController(trafficLight, pedestrianLight, pedestrianButton, buzzer, &serialReporter);
WiFiService wifiService(WIFI_SSID, WIFI_PASSWORD);
WebServerService webServer(&trafficController);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  trafficController.begin();
  wifiService.begin();
  webServer.begin();
}

void loop() {
#if DEBUG_BUTTON
  static int lastLevel = -1;
  int level = digitalRead(BUTTON_PEDESTRIAN);
  if (level != lastLevel) {
    lastLevel = level;
    Serial.print("Button level: ");
    Serial.println(level == HIGH ? "HIGH" : "LOW");
  }
#endif
  trafficController.update();
}
