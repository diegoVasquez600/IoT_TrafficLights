#include <Arduino.h>
#include <lights/TrafficLight.h>
#include <lights/PedestrianLight.h>
#include <devices/Button.h>
#include <devices/Buzzer.h>
#include <controllers/TrafficController.h>
#include <controllers/SerialReporter.h>

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



TrafficLight trafficLight(LED_RED, LED_YELLOW, LED_GREEN);
PedestrianLight pedestrianLight(LED_PEDESTRIAN_GREEN, LED_PEDESTRIAN_RED);
Button pedestrianButton(BUTTON_PEDESTRIAN);
Buzzer buzzer(BUZZER_PIN);
SerialReporter serialReporter;

TrafficController trafficController(trafficLight, pedestrianLight, pedestrianButton, buzzer, &serialReporter);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  trafficController.begin();
}

void loop() {
  trafficController.update();
}
