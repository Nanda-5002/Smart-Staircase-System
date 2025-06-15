#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 25  // Number of LEDs per strip
#define NUM_STRIPS 5 // Total number of LED strips
#define LDR_PIN A0   // LDR connected to analog pin A0

// Define digital pins for LED strips and IR sensors
const int stripPins[NUM_STRIPS] = {2, 3, 4, 5, 6};
const int irSensorPins[NUM_STRIPS] = {8,9,10,11,12}; // IR sensors for each step

// Create an array of NeoPixel objects
Adafruit_NeoPixel strips[NUM_STRIPS] = {
    Adafruit_NeoPixel(NUM_LEDS, stripPins[0], NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, stripPins[1], NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, stripPins[2], NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, stripPins[3], NEO_GRB + NEO_KHZ800),
    Adafruit_NeoPixel(NUM_LEDS, stripPins[4], NEO_GRB + NEO_KHZ800)
    
};

// Brightness levels
const int FULL_BRIGHTNESS = 255;
const int DIM_BRIGHTNESS = 50;
const int OFF_BRIGHTNESS = 0;

const int ldrThreshold = 500; // Adjust based on your room brightness
int lastActivatedStep = -1;

void setup() {
    Serial.begin(9600);

    for (int i = 0; i < NUM_STRIPS; i++) {
        pinMode(irSensorPins[i], INPUT);
        strips[i].begin();
        strips[i].clear();
        strips[i].show();
    }

    pinMode(LDR_PIN, INPUT);
}

void loop() {
    int ldrValue = analogRead(LDR_PIN);
    bool isDark = (ldrValue < ldrThreshold);
    bool motionDetected = false;

    // Only check motion if it's dark
    if (isDark) {
        for (int i = 0; i < NUM_STRIPS; i++) {
            if (digitalRead(irSensorPins[i]) == LOW) {
                lastActivatedStep = i;
                motionDetected = true;
                break;
            }
        }
    }

    if (!motionDetected) {
        turnOffAllLEDs();
        lastActivatedStep = -1;
    } else {
        for (int i = 0; i < NUM_STRIPS; i++) {
            int brightness;
            if (i == lastActivatedStep) {
                brightness = FULL_BRIGHTNESS;
            } else if (i == lastActivatedStep - 1) {
                brightness = DIM_BRIGHTNESS;
            } else {
                brightness = OFF_BRIGHTNESS;
            }

            for (int j = 0; j < NUM_LEDS; j++) {
                uint8_t r = (255 * brightness) / 255;
                uint8_t g = (150 * brightness) / 255;
                uint8_t b = (0 * brightness) / 255;

                strips[i].setPixelColor(j, strips[i].Color(r, g, b));
            }
            strips[i].show();
        }
    }
}

void turnOffAllLEDs() {
    for (int i = 0; i < NUM_STRIPS; i++) {
        strips[i].clear();
        strips[i].show();
    }
}