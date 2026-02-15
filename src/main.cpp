#include <Arduino.h>
#include <lights/TrafficLight.h>
#include <lights/PedestrianLight.h>
#include <devices/Button.h>
#include <devices/Buzzer.h>
#include <controllers/TrafficController.h>

#define LED_GREEN 25
#define LED_YELLOW 26
#define LED_RED 27

#define LED_PEDESTRIAN_RED 33
#define LED_PEDESTRIAN_GREEN 32

#define SERIAL_BAUD_RATE 9600
#define BUTTON_PEDESTRIAN 17

TrafficLight trafficLight(LED_RED, LED_YELLOW, LED_GREEN);
PedestrianLight pedestrianLight(LED_PEDESTRIAN_GREEN, LED_PEDESTRIAN_RED);
Button pedestrianButton(BUTTON_PEDESTRIAN);
Buzzer buzzer(16);

TrafficController trafficController(trafficLight, pedestrianLight, pedestrianButton, buzzer);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  trafficController.begin();
}

void loop() {
  trafficController.update();
}
