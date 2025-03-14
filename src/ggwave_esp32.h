#ifndef GGWAVE_ESP32_H
#define GGWAVE_ESP32_H

#include <Arduino.h>

// Constants for audio modulation
#define GGWAVE_SAMPLE_RATE 16000
#define GGWAVE_SAMPLE_SIZE 512
#define GGWAVE_TX_FREQ_START 1000
#define GGWAVE_TX_FREQ_STEP 100
#define GGWAVE_NUM_FREQUENCIES 16
#define GGWAVE_PROTOCOL_VERSION 1

// Packet format constants
#define PACKET_START_MARKER 0xFF
#define PACKET_END_MARKER 0xFE
#define MAX_PACKET_SIZE 256

class GGWaveESP32 {
public:
    GGWaveESP32();
    
    // Initialization
    void begin();
    
    // Modulation
    bool modulateData(const char* data, size_t length, int16_t* audioBuffer, size_t* audioLength);
    
    // Demodulation
    bool demodulateAudio(const int16_t* audioBuffer, size_t length, char* data, size_t* dataLength);

    // Web interface integration
    static String generateJavaScript();

private:
    // Helper functions for modulation
    void generateTone(int16_t* buffer, size_t length, float frequency, float amplitude);
    uint8_t encodeByte(char byte);
    void addPacketHeader(int16_t* buffer, size_t* offset);
    void addPacketFooter(int16_t* buffer, size_t* offset);
    
    // Helper functions for demodulation
    void performFFT(const int16_t* input, float* output, size_t length);
    char decodeByte(const float* frequencies);
    bool validatePacket(const char* data, size_t length);
    
    // Lookup table for sine values
    static const uint16_t sinTable[256];
};

#endif // GGWAVE_ESP32_H 