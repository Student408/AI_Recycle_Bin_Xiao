#include <PDM.h>

#define SAMPLE_RATE 16000
#define AUDIO_BUFFER_SIZE 256

short sampleBuffer[AUDIO_BUFFER_SIZE];
volatile int samplesRead;

unsigned long recordingStartTime;
const unsigned long RECORDING_DURATION = 10000; // 10 seconds

void setup() {
  Serial.begin(115200);
  while (!Serial);

  PDM.onReceive(onPDMdata);
  if (!PDM.begin(1, SAMPLE_RATE)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }

  // Send header information
  Serial.println("AUDIO_START");
  Serial.println(SAMPLE_RATE);
  Serial.println(RECORDING_DURATION);

  recordingStartTime = millis();
}

void loop() {
  if (samplesRead) {
    // Send the audio samples over serial
    Serial.write((const uint8_t *)sampleBuffer, samplesRead * 2);
    samplesRead = 0;
  }

  // Check if recording duration has elapsed
  if (millis() - recordingStartTime >= RECORDING_DURATION) {
    Serial.println("AUDIO_END");
    while (1); // Stop the program
  }
}

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}
