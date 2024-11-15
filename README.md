# AI Audio Classifier Recycle Bin

This project implements an AI-based sorting system using an Edge Impulse audio classifier to identify different types of trash. It uses a microphone to capture sound, runs the audio classification model to detect the type of object (e.g., bottle, can, paper, or noise), and then sorts the object into one of several compartments using a stepper motor and servo.

## Features

- **Microphone-based object classification**: Classifies incoming audio signals to determine the type of trash (e.g., bottle, can, paper, or noise).
- **Automatic sorting**: Uses a stepper motor and servo to move the detected object into the corresponding compartment.
- **IR sensor integration**: If the classifier detects "noise", an IR sensor is used to trigger sorting for unidentified objects.
- **Edge Impulse AI Model**: The classification is powered by an Edge Impulse machine learning model.

## Hardware Requirements

- **Microcontroller**: Any compatible board (e.g., Arduino Nano 33 BLE Sense).
- **Microphone**: For capturing audio data (PDM microphone recommended).
- **Stepper Motor**: For moving the compartments to sort the detected objects.
- **Servo Motor**: For opening and closing the trapdoor.
- **IR Sensor**: For detecting objects not classified by the model.
- **Hall Sensor**: For stepper motor homing/calibration.

## Components

- 1x Microcontroller (e.g., Arduino Nano 33 BLE Sense)
- 1x Stepper motor (with driver)
- 1x Servo motor (for trapdoor mechanism)
- 1x IR sensor
- 1x Hall effect sensor (for stepper motor homing)
- 1x PDM microphone (for audio input)

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-username/ai-audio-classifier-recycle-bin.git
   cd ai-audio-classifier-recycle-bin
   ```

2. **Install the required libraries**:
   - `AccelStepper`: For controlling the stepper motor.
   - `Servo`: For controlling the servo motor.
   - `Edge Impulse` Arduino library: For running the classification model.

   You can install these libraries using the Arduino Library Manager or by manually adding them to your project.

3. **Set up the Edge Impulse model**:
   - Use the [Edge Impulse Studio](https://www.edgeimpulse.com/) to create and train a model for classifying trash sounds (e.g., bottle, can, paper, noise).
   - Export the model for Arduino and include the generated files in the project (e.g., `AI_Recycle_Bin_Xiao__inferencing.h`).

4. **Upload the code**:
   - Open the project in the Arduino IDE.
   - Select the correct board and port.
   - Upload the sketch to your microcontroller.

## Usage

1. Once the system is powered up, the program will initialize and calibrate the stepper motor and servo.
2. The microphone starts recording audio, and the Edge Impulse model classifies the detected sound.
3. Based on the classification result, the stepper motor moves to the corresponding compartment and the servo opens the trapdoor to drop the object.
4. If the object is unidentified (detected as noise), the IR sensor can trigger the sorting into a specific compartment.

### Sensor States:
- **Hall Sensor**: Used for stepper motor calibration.
- **IR Sensor**: Detects objects when the classifier fails to categorize them as noise.
- **Stepper Motor**: Moves the compartments for sorting.
- **Servo Motor**: Controls the trapdoor mechanism.

## Code Overview

- **Microphone Setup**: Initializes the microphone for audio input using PDM (Pulse Density Modulation).
- **Edge Impulse Inferencing**: Runs the trained AI model to classify audio signals and detect the type of object.
- **Stepper Motor Control**: Moves the stepper motor to the appropriate compartment for sorting.
- **Servo Control**: Opens and closes the trapdoor to drop the sorted object.

## Example Output

When an object is classified, you will see output similar to the following in the Serial Monitor:

```
Starting inferencing...
Recording...
Recording done
Predictions:
    Bottle: 0.92
    Can: 0.03
    Paper: 0.02
    Noise: 0.00
YOUR TRASH IS: Bottle
Moving stepper to selected compartment...
Opening trapdoor...
...
Closing trapdoor...
```

## License

This project is licensed under the [Apache 2.0 License](LICENSE).

## Acknowledgements

- [Edge Impulse](https://www.edgeimpulse.com/) for the machine learning platform.
- [AccelStepper](https://www.airspayce.com/mikem/arduino/AccelStepper/) for stepper motor control.
- [Servo Library](https://www.arduino.cc/en/Reference/Servo) for servo motor control.

---

Feel free to modify and improve this system to suit your needs. Contributions are welcome!
