#ifndef ESP32_GIBBERLINK_H
#define ESP32_GIBBERLINK_H

#include <Arduino.h>
#include "driver/i2s.h"
#include "ggwave_esp32.h"

// Audio recording constants
#define SAMPLE_RATE     16000
#define SAMPLE_BITS     16
#define SAMPLE_CHANNELS 1
#define BUFFER_SIZE     512

// Audio output types
#define AUDIO_OUTPUT_DAC 1  // Direct speaker connection via DAC
#define AUDIO_OUTPUT_I2S 2  // I2S amplifier connection

/**
 * ESP32GibberLink - A library for transmitting data over audio using the ESP32
 * 
 * This library implements the GibberLink protocol for transmitting data over audio waves.
 * It supports both direct speaker connection via DAC and I2S amplifier output.
 */
class ESP32GibberLink {
public:
    ESP32GibberLink();
    ~ESP32GibberLink();
    
    // Initialization methods
    bool begin(int micPin, int speakerPin);  // For DAC output
    bool beginI2S(int micPin, int bclkPin, int wsPin, int dataPin);  // For I2S output
    
    // Audio functions
    bool startRecording();
    bool stopRecording();
    bool sendData(const char* data, size_t length);
    
    // Callback for received data
    typedef void (*DataCallback)(const char* data, size_t length);
    void setDataCallback(DataCallback callback);

private:
    // I2S configuration
    i2s_config_t i2s_config;
    i2s_pin_config_t i2s_pins;
    
    // Microphone and output pins
    int _micPin;
    int _speakerPin;  // For DAC
    int _bclkPin;     // For I2S
    int _wsPin;       // For I2S
    int _dataPin;     // For I2S
    
    // Audio output type
    uint8_t _outputType;
    
    // Callback function
    DataCallback _callback;
    
    // Audio buffer
    int16_t* _audioBuffer;
    size_t _audioBufferSize;
    
    // GGWave instance
    GGWaveESP32 _ggwave;
    
    // Recording task handle
    TaskHandle_t _recordingTask;
    
    // Helper functions
    void configureI2S();
    void configureI2SOutput();
    void processAudioData(int16_t* samples, size_t count);
    static void recordingTaskFunction(void* parameter);
    bool playAudioBuffer(const int16_t* buffer, size_t length);
};

#endif // ESP32_GIBBERLINK_H 