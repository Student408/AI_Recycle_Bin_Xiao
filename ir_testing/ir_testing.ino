#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

// Pin definitions
const int IR_SENSOR_PIN = 9;      // IR sensor input pin
const int BUILTIN_LED = LED_BLUE; // Built-in LED on XIAO nRF52840 Sense
const int SAMPLE_INTERVAL = 500;  // Reading interval in milliseconds

// Variables for detection state
bool lastDetectionState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
    pinMode(IR_SENSOR_PIN, INPUT);
    pinMode(BUILTIN_LED, OUTPUT);
    
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.begin(115200);

    unsigned long startMillis = millis();
    while (!Serial && (millis() - startMillis < 5000)) {
        delay(100);
    }

    digitalWrite(BUILTIN_LED, LOW);
    delay(1000);
    
    Serial.println("\nIR Sensor State Monitor");
    Serial.println("----------------------");
    Serial.println("Using XIAO nRF52840 Sense");
    Serial.println("IR Sensor connected to pin 9");
    Serial.println("Starting continuous monitoring...\n");
    
    // Print header for status display
    Serial.println("Format: [Raw State] | Detection Status");
    Serial.println("--------------------------------");
}

void loop() {
    static unsigned long lastPrintTime = 0;
    int irState = digitalRead(IR_SENSOR_PIN);
    bool detectionState = (irState == HIGH);
    
    // Update detection state with debouncing
    if (detectionState != lastDetectionState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (detectionState != lastDetectionState) {
            // Update LED state
            digitalWrite(BUILTIN_LED, detectionState);
            
            // Print state change with raw value
            Serial.print("[");
            Serial.print(irState ? "HIGH" : "LOW");
            Serial.print("] | ");
            if (detectionState) {
                Serial.println("DETECTED: Object in range");
            } else {
                Serial.println("CLEARED: No object detected");
            }
            
            Serial.flush();
            lastDetectionState = detectionState;
        }
        
        // Periodic status update with raw state
        if (millis() - lastPrintTime >= SAMPLE_INTERVAL) {
            Serial.print("[");
            Serial.print(irState ? "HIGH" : "LOW");
            Serial.print("] | Status: ");
            Serial.println(detectionState ? "Object Present" : "Clear");
            Serial.flush();
            lastPrintTime = millis();
        }
    }
    
    delay(10);
}