#include <OBD2UART.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN      6
#define NUM_LEDS     30
#define BRIGHTNESS   255

#define MAX_RPM      9000
#define IDLE_RPM     600
#define WARN_RPM     7000

COBD obd;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

float displayLevel = 0;
float velocity = 0;

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.clear();
  strip.show();

  Serial.println("Connecting to OBD");
  obd.begin();
  while (!obd.init()) {
    Serial.println("Retrying");
    delay(500);
  }
  Serial.println("OBD Connected");
}

void loop() {
  int rpm = 0;
  obd.readPID(PID_RPM, rpm);

  Serial.print("RPM: ");
  Serial.println(rpm);

  updateRPM(rpm);
  strip.show();
}

void updateRPM(int rpm) {
  if (rpm >= WARN_RPM) {
    uint8_t strobe = (millis() / 60) % 2 ? 255 : 180;
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(strobe, strobe, strobe));
    }
    return;
  }

  float target = 0;
  if (rpm >= IDLE_RPM) {
    target = constrain((float)(rpm - IDLE_RPM) / (MAX_RPM - IDLE_RPM), 0.0, 1.0);
  }

  float diff = target - displayLevel;
  velocity += diff > 0 ? diff * 0.5 : diff * 0.2;
  velocity *= 0.7;
  displayLevel += velocity;
  displayLevel = constrain(displayLevel, 0.0, 1.0);

  int numLit = constrain((int)(displayLevel * NUM_LEDS), 0, NUM_LEDS);

  strip.clear();
  for (int i = 0; i < numLit; i++) {
    float t = (float)i / (NUM_LEDS - 1);
    uint8_t r, g, b;
    if (t < 0.5) {
      r = (uint8_t)(100 + t * 2 * 105);
      g = 0;
      b = (uint8_t)(255 - t * 2 * 80);
    } else {
      r = (uint8_t)(205 + (t - 0.5) * 2 * 50);
      g = (uint8_t)((t - 0.5) * 2 * 20);
      b = (uint8_t)(175 - (t - 0.5) * 2 * 175);
    }
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  if (numLit > 0) {
    strip.setPixelColor(numLit - 1, strip.Color(255, 255, 255));
  }
}