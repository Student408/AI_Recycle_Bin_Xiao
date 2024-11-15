# AI Audio Classifier Recycle Bin

This project is an edge AI-based solution for sorting recyclable waste items (e.g., bottles, cans, paper) using an AI model to classify audio signals. The system uses an AI classifier running on a microcontroller, coupled with a microphone and stepper motor for sorting waste based on the detected sound. The system also integrates a servo motor to release waste into the appropriate compartment.

## Features

- **Real-time Audio Classification**: Uses an AI model to classify audio signals from a microphone, distinguishing between different types of recyclable materials (e.g., bottles, cans, paper, noise).
- **Stepper Motor Control**: Moves waste to the correct compartment based on classification.
- **Servo Motor Control**: Opens and closes a trapdoor for each compartment, releasing the waste.
- **IR Sensor Integration**: Detects unidentified objects or noise and activates the sorting mechanism if needed.
- **Hall Sensor for Stepper Homing**: Ensures proper stepper motor positioning using a hall sensor.

## Hardware Requirements

1. **Microcontroller**: Compatible with Edge Impulse and supports PDM microphone input (e.g., STM32, Teensy).
2. **PDM Microphone**: Used for recording audio input for classification.
3. **Stepper Motor**: For controlling the sorting mechanism.
4. **Servo Motor**: For operating the trapdoor for waste sorting.
5. **IR Sensor**: For detecting unidentified objects or triggering sorting actions.
6. **Hall Sensor**: For stepper motor homing.

## Software Requirements

- **Edge Impulse Studio**: To train the model and export it for deployment to the microcontroller.
- **Arduino IDE**: For uploading the code to the microcontroller.

## Setup

### 1. Connect the Hardware
- **Microphone**: Connect the PDM microphone to the microcontroller.
- **Stepper Motor**: Connect the stepper motor to the specified pins.
- **Servo Motor**: Connect the servo motor to the defined servo pin.
- **IR Sensor**: Connect the IR sensor to the designated input pin.
- **Hall Sensor**: Connect the hall sensor to the microcontroller for stepper motor homing.

### 2. Edge Impulse Model
- **Train the AI Model**: Use Edge Impulse Studio to create a sound classification model to detect different recyclable materials.
- **Export the Model**: Once the model is trained, export the classifier to be used in the microcontroller.

### 3. Upload the Code
- Open the Arduino IDE and upload the code to your microcontroller.

### 4. Run the System
- The system will continuously record audio, classify it using the AI model, and sort the waste into predefined compartments based on the detected classification.

## Code Overview

- **`setup()`**: Initializes the stepper, servo, and microphone. It also starts the inferencing process and prepares the system for audio classification.
- **`loop()`**: Continuously records audio, runs the classifier on the recorded audio, and sorts the waste based on classification results. It also checks the IR sensor for unidentified objects.
- **Inferencing Logic**: The system uses the `microphone_inference_*` functions to collect and process audio data, which is then passed to the AI model for classification.
- **Motor Control**: The stepper motor moves to the appropriate compartment based on the classification, and the servo motor opens/closes the trapdoor to release the waste.

## Customization

- **Threshold**: The threshold value for classification confidence can be adjusted via the `threshold` variable. If a class prediction exceeds this value, it triggers the sorting action.
- **Compartment Positions**: The `compartment[]` array defines the positions for the stepper motor corresponding to each type of recyclable material.

## Troubleshooting

- **IR Sensor Issues**: If the system doesn't detect objects correctly, check the IR sensor wiring and positioning.
- **Stepper Motor Issues**: Ensure the hall sensor is working for stepper motor homing. Adjust stepper motor parameters if needed.
- **Audio Classification**: If classifications are not accurate, retrain the model in Edge Impulse Studio using better data or fine-tuning the model parameters.

## License

This project is licensed under the [Apache 2.0 License](https://www.apache.org/licenses/LICENSE-2.0).

---
