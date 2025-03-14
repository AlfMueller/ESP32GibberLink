# ESP32 GibberLink

> [!IMPORTANT]
> This is a work-in-progress ESP32 port of the [GibberLink](https://github.com/PennyroyalTea/gibberlink) library for transmitting data over audio waves. While basic functionality is working, some features may be incomplete or subject to change.

## Status

🚧 **Under Development** 🚧
- Basic functionality is implemented
- Some features may be incomplete
- API may change in future versions
- Testing and optimization ongoing

## Features

- Data transmission over audio waves
- Web interface for sending messages
- Support for both direct speaker connection (DAC) and I2S amplifier
- Compatible with all ASCII characters
- Easy integration into existing projects
- Web-based sender included

## Hardware Requirements

### Basic Setup (DAC output)
- ESP32 Development Board
- I2S MEMS Microphone (e.g., INMP441)
- 8Ω speaker or piezo buzzer
- 100Ω resistor

### Enhanced Setup (I2S output)
- ESP32 Development Board
- I2S MEMS Microphone (e.g., INMP441)
- MAX98357A I2S Amplifier
- Speaker (4Ω or 8Ω)

## Pin Connections

### Microphone (INMP441)
```
ESP32    INMP441
3.3V --- VDD
GND  --- GND
GPIO34 - SD (Data)
GND  --- L/R
GPIO25 - WS (Word Select)
GPIO26 - SCK (Serial Clock)
```

### Speaker Options

#### 1. Direct DAC Connection
```
ESP32              Speaker
GPIO25 --- 100Ω --- Positive
GND   ------------ Negative
```

#### 2. I2S Amplifier (MAX98357A)
```
ESP32         MAX98357A
GPIO26 ------ BCLK
GPIO25 ------ LRC
GPIO22 ------ DIN
3.3V  ------ VIN
GND   ------ GND
```

## Installation

1. Download this repository as ZIP
2. In Arduino IDE: Sketch -> Include Library -> Add .ZIP Library
3. Select the downloaded ZIP file

## Usage

### Basic Example
```cpp
#include <ESP32GibberLink.h>

ESP32GibberLink gibberlink;

void onDataReceived(const char* data, size_t length) {
    Serial.println("Received: ");
    Serial.write((uint8_t*)data, length);
}

void setup() {
    // Initialize with DAC output
    gibberlink.begin(MIC_PIN, SPEAKER_PIN);
    // Or initialize with I2S output
    // gibberlink.beginI2S(MIC_PIN, I2S_BCLK, I2S_LRC, I2S_DOUT);
    
    gibberlink.setDataCallback(onDataReceived);
    gibberlink.startRecording();
}
```

### Web Interface

The library includes a web interface example for sending messages from any browser:

1. Upload the `web_sender` example
2. Connect to the ESP32's WiFi network
3. Open the displayed IP address in your browser
4. Enter text and click "Send"

## Configuration

In `web_sender.ino`, you can select the audio output type:
```cpp
// Choose your audio output type
#define AUDIO_OUTPUT_TYPE  AUDIO_OUTPUT_DAC  // Simple speaker
// or
#define AUDIO_OUTPUT_TYPE  AUDIO_OUTPUT_I2S  // I2S amplifier
```

## Protocol Details

The audio protocol uses:
- Sample rate: 16kHz
- Bit depth: 16-bit
- Frequency range: 1000Hz - 2600Hz
- Bit duration: 10ms
- Error detection: Basic packet validation

## Limitations

- Maximum message length: 256 bytes
- Transmission speed: ~100 bytes/second
- Range depends on volume and environmental noise
- Best results in quiet environments

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Original [GibberLink project](https://github.com/PennyroyalTea/gibberlink)
- [ggwave library](https://github.com/ggerganov/ggwave) by Georgi Gerganov
- ESP32 Arduino Core developers
