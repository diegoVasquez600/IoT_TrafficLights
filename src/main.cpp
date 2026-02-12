#include <Arduino.h>

#define LED_GREEN 25
#define LED_YELLOW 26
#define LED_RED 27

#define LED_PEDESTRIAN_RED 33
#define LED_PEDESTRIAN_GREEN 32

#define SERIAL_BAUD_RATE 9600
#define BUTTON_PEDESTRIAN 17

// --- tiempos normal ---
const unsigned long greenDuration = 15000;
const unsigned long yellowDuration = 5000;
const unsigned long redDuration = 15000;

// --- botón / debounce ---
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 200;

// ajusta si tu módulo entrega HIGH al presionar
const bool PRESSED_STATE = LOW;

// --- override peatón ---
bool pedestrianOverride = false;
unsigned long pedOverrideStart = 0;
const unsigned long pedestrianDuration = 20000; // 20 segundos

// para “reanudar” el ciclo sin saltos
unsigned long cycleStartMillis = 0;

int lastPhase = -1; // 0=green,1=yellow,2=red

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(LED_PEDESTRIAN_RED, OUTPUT);
  pinMode(LED_PEDESTRIAN_GREEN, OUTPUT);

  pinMode(BUTTON_PEDESTRIAN, INPUT); // si es botón simple a GND, usa INPUT_PULLUP

  cycleStartMillis = millis();

  // estado inicial
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_PEDESTRIAN_RED, HIGH);
  digitalWrite(LED_PEDESTRIAN_GREEN, LOW);
}

void applyPedestrianOverride() {
  // Vehículos: rojo
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, HIGH);

  // Peatón: verde
  digitalWrite(LED_PEDESTRIAN_RED, LOW);
  digitalWrite(LED_PEDESTRIAN_GREEN, HIGH);
}

void trafficLightSequence() {
  unsigned long now = millis();

  // Si override está activo, manda y sale
  if (pedestrianOverride) {
    applyPedestrianOverride();

    if (now - pedOverrideStart >= pedestrianDuration) {
      pedestrianOverride = false;
      Serial.println("Pedestrian time finished -> resuming normal cycle");

      // Reinicia el ciclo para continuar "normal" desde verde
      cycleStartMillis = now;
      lastPhase = -1; // fuerza impresión/cambio en la siguiente vuelta
    }
    return;
  }

  // --- ciclo normal basado en cycleStartMillis ---
  unsigned long elapsed = now - cycleStartMillis;
  unsigned long totalCycle = greenDuration + yellowDuration + redDuration;
  unsigned long cyclePosition = elapsed % totalCycle;

  int phase;
  if (cyclePosition < greenDuration) phase = 0;
  else if (cyclePosition < greenDuration + yellowDuration) phase = 1;
  else phase = 2;

  // actualizar LEDs (siempre)
  if (phase == 0) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_PEDESTRIAN_RED, HIGH);
    digitalWrite(LED_PEDESTRIAN_GREEN, LOW);
  } else if (phase == 1) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_PEDESTRIAN_RED, HIGH);
    digitalWrite(LED_PEDESTRIAN_GREEN, LOW);
  } else {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_PEDESTRIAN_RED, LOW);
    digitalWrite(LED_PEDESTRIAN_GREEN, HIGH);
  }

  // imprimir solo cuando cambie fase
  if (phase != lastPhase) {
    lastPhase = phase;
    if (phase == 0) Serial.println("NORMAL: GREEN (vehicles) / RED (ped)");
    else if (phase == 1) Serial.println("NORMAL: YELLOW (vehicles) / RED (ped)");
    else Serial.println("NORMAL: RED (vehicles) / RED (ped)");
  }
}

void checkPedestrianButton() {
  bool reading = digitalRead(BUTTON_PEDESTRIAN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // activa override si NO está ya activo
    if (reading == PRESSED_STATE && !pedestrianOverride) {
      pedestrianOverride = true;
      pedOverrideStart = millis();
      Serial.println("BUTTON: Pedestrian override ON (20s) -> vehicles RED / ped GREEN");
    }
  }

  lastButtonState = reading;
}

void loop() {
  checkPedestrianButton();
  trafficLightSequence();
}
