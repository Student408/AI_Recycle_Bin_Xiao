import serial
import wave
import struct
import time

# Configure the serial port
ser = serial.Serial('COM7', 115200)  # Replace 'COM3' with your Arduino's port

# Wait for the "AUDIO_START" signal
while ser.readline().decode().strip() != "AUDIO_START":
    pass

# Read sample rate and duration
sample_rate = int(ser.readline().decode().strip())
duration = int(ser.readline().decode().strip()) / 1000  # Convert to seconds

# Open a new WAV file
with wave.open('recording.wav', 'wb') as wav_file:
    wav_file.setnchannels(1)  # Mono
    wav_file.setsampwidth(2)  # 2 bytes per sample
    wav_file.setframerate(sample_rate)

    start_time = time.time()
    while time.time() - start_time < duration:
        if ser.in_waiting >= 512:  # Read in chunks of 256 samples
            data = ser.read(512)
            wav_file.writeframes(data)

    # Read any remaining data
    remaining_data = ser.read_all()
    wav_file.writeframes(remaining_data)

print("Recording saved as recording.wav")
ser.close()