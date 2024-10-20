/* AI Audio Classifier Recycle Bin
 * License: Apache-2.0
 * Modification by: Samuel Alexander
 * Based on the nano_ble33_sense_microphone example
 */

#define EIDSP_QUANTIZE_FILTERBANK   0

/* Includes ---------------------------------------------------------------- */
#include <PDM.h>
#include <AI_Recycle_Bin_Xiao__inferencing.h>
#include <AccelStepper.h>
#include <Servo.h>

/** Audio buffers, pointers and selectors */
typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;

static inference_t inference;
static signed short sampleBuffer[2048];
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal

AccelStepper stepper1(1, 7, 8); // (Typeof driver: with 2 pins, STEP, DIR)

int compartment[] = {50, 100, 150, 200, 0}; // Based on our EI model (Bottle, Can, Paper, Pong, Noise)
const int HALL = A1;
const int SERVO_PIN = 6; // Defining servo signal pin
const int IR_SENSOR_PIN = 9; // Define pin for IR sensor

Servo trapdoor;
int servoPos = 11; // Initial servo position
float threshold = 0.8;
int selected = 4; // Default as Noise

void setup()
{
    Serial.begin(115200);
    pinMode(HALL, INPUT);
    pinMode(IR_SENSOR_PIN, INPUT); // Set IR sensor pin as input
    trapdoor.attach(SERVO_PIN); // Using the defined constant
    
    // Stepper motor calibration
    stepper1.setMaxSpeed(2000);
    stepper1.setAcceleration(1000);
    while(digitalRead(HALL)){
      stepper1.setSpeed(-400);
      stepper1.runSpeed();
      Serial.println("homing...");
    }
    delay(100);
    stepper1.setCurrentPosition(0);

    // Servo motor calibration
    for (servoPos = 11; servoPos <= 180; servoPos += 1) {
        trapdoor.write(servoPos);
        delay(20);
    }
    for (servoPos = 180; servoPos >= 11; servoPos -= 1) {
        trapdoor.write(servoPos);
        delay(20);
    }
    delay(500);

    Serial.println("Edge Impulse Inferencing Demo");

    // Summary of inferencing settings
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));

    if (microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT) == false) {
        ei_printf("ERR: Could not allocate audio buffer (size %d)\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
        return;
    }

    // Print initial sensor states
    printSensorStates("Initial");
}

void loop()
{
    ei_printf("\nStarting inferencing...\n");

    ei_printf("Recording...\n");

    bool m = microphone_inference_record();
    if (!m) {
        ei_printf("ERR: Failed to record audio...\n");
        return;
    }

    ei_printf("Recording done\n");

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = { 0 };

    EI_IMPULSE_ERROR r = run_classifier(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", r);
        return;
    }

    // Print predictions
    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)", result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");
    selected = 4; // Default to "Noise"
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
        if (result.classification[ix].value > threshold) {
            ei_printf("\nYOUR TRASH IS: %s\n", result.classification[ix].label);
            selected = ix;
        }
    }

    // Check IR sensor if no valid category was detected
    if (selected == 4 && digitalRead(IR_SENSOR_PIN) == LOW ) {
        Serial.println("\nUnidentified object detected by IR sensor.");
        selected = 3; // Move to the Pong compartment (6475 steps)
    }

    // Print sensor data before sorting
    printSensorStates("Before Sorting");

    if (selected != 4) { // If not NOISE
        // Move stepper to the selected compartment
        Serial.println("\nMoving stepper to selected compartment...");
        stepper1.moveTo(compartment[selected]);
        while (stepper1.distanceToGo() != 0) {
            stepper1.run();
            if (stepper1.distanceToGo() % 100 == 0) { // Print every 100 steps
                Serial.print("Stepper Position: ");
                Serial.println(stepper1.currentPosition());
            }
        }
        
        Serial.println("\nOpening trapdoor...");
        for (servoPos = 11; servoPos <= 180; servoPos += 1) {
            trapdoor.write(servoPos);
            delay(20);
            if (servoPos % 10 == 0) { // Print every 10 degrees
                Serial.print("Servo Position: ");
                Serial.println(servoPos);
            }
        }
        
        delay(1000); // Wait for item to fall through
        
        Serial.println("\nClosing trapdoor...");
        for (servoPos = 180; servoPos >= 11; servoPos -= 1) {
            trapdoor.write(servoPos);
            delay(20);
            if (servoPos % 10 == 0) { // Print every 10 degrees
                Serial.print("Servo Position: ");
                Serial.println(servoPos);
            }
        }
        
        // Print final positions
        printSensorStates("After Sorting");
        
        delay(500);
    } else {
        Serial.println("\nNoise detected. No sorting action taken.");
    }

#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif
}

void printSensorStates(const char* stage) {
    Serial.print("\n");
    Serial.print(stage);
    Serial.println(" Sensor States:");
    Serial.print("Hall Sensor State: ");
    Serial.println(digitalRead(HALL));
    Serial.print("IR Sensor State: ");
    Serial.println(digitalRead(IR_SENSOR_PIN));
    Serial.print("Stepper Position: ");
    Serial.println(stepper1.currentPosition());
    Serial.print("Servo Position: ");
    Serial.println(servoPos);
}

/**
 * @brief      PDM buffer full callback
 *             Get data and call audio thread callback
 */
static void pdm_data_ready_inference_callback(void)
{
    int bytesAvailable = PDM.available();

    // Read into the sample buffer
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);

    if (inference.buf_ready == 0) {
        for (int i = 0; i < bytesRead >> 1; i++) {
            inference.buffer[inference.buf_count++] = sampleBuffer[i];

            if (inference.buf_count >= inference.n_samples) {
                inference.buf_count = 0;
                inference.buf_ready = 1;
                break;
            }
        }
    }
}

/**
 * @brief      Init inferencing struct and setup/start PDM
 */
static bool microphone_inference_start(uint32_t n_samples)
{
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));

    if (inference.buffer == NULL) {
        return false;
    }

    inference.buf_count = 0;
    inference.n_samples = n_samples;
    inference.buf_ready = 0;

    // Configure the data receive callback
    PDM.onReceive(&pdm_data_ready_inference_callback);

    PDM.setBufferSize(4096);

    // Initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate
    if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY)) {
        ei_printf("Failed to start PDM!");
        microphone_inference_end();

        return false;
    }

    // Set the gain, defaults to 20
    PDM.setGain(80);

    return true;
}

/**
 * @brief      Wait on new data
 */
static bool microphone_inference_record(void)
{
    inference.buf_ready = 0;
    inference.buf_count = 0;

    while (inference.buf_ready == 0) {
        delay(10);
    }

    return true;
}

/**
 * Get raw audio signal data
 */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr)
{
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);

    return 0;
}

/**
 * @brief      Stop PDM and release buffers
 */
static void microphone_inference_end(void)
{
    PDM.end();
    free(inference.buffer);
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor."
#endif
