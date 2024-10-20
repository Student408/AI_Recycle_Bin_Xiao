#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // Add TinyUSB Library

// Pin definitions
const int IR_SENSOR_PIN = 9;     // IR sensor input pin
const int BUILTIN_LED = LED_BLUE; // Built-in LED on XIAO nRF52840 Sense
const int SAMPLE_INTERVAL = 500;  // Reading interval in milliseconds

// Variables for detection state
bool lastDetectionState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Debounce time in milliseconds

void setup() {
    // Initialize USB Serial
    Serial.begin(115200);
    
    // Simple wait for USB Serial connection
    delay(2000);
    
    // Configure pins
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(BUILTIN_LED, OUTPUT);
    
    Serial.println("IR Sensor Detection System");
    Serial.println("-------------------------");
    Serial.println("Using XIAO nRF52840 Sense");
    Serial.println("IR Sensor connected to pin 5");
}

void loop() {
    static unsigned long lastPrintTime = 0;
    int irState = digitalRead(IR_SENSOR_PIN);
    bool detectionState = (irState == HIGH);
    
    // Implement debouncing
    if (detectionState != lastDetectionState) {
        lastDebounceTime = millis();
    }
    
    // If state has been stable for debounce period
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Only update if detection state has changed
        if (detectionState != lastDetectionState) {
            // Update LED state
            digitalWrite(BUILTIN_LED, detectionState);
            
            // Print status message if enough time has passed
            if (millis() - lastPrintTime >= SAMPLE_INTERVAL) {
                if (detectionState) {
                    Serial.println("Object detected by IR sensor");
                } else {
                    Serial.println("No object detected by IR sensor");
                }
                lastPrintTime = millis();
            }
            
            lastDetectionState = detectionState;
        }
    }
    
    // Small delay to prevent overwhelming the serial output
    delay(10);
}