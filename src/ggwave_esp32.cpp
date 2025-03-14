#include "ggwave_esp32.h"
#include <math.h>

// Pre-calculated sine table for faster computation
const uint16_t GGWaveESP32::sinTable[256] = {
    // ... (will be filled with actual values later)
    32768, 33572, 34368, 35137, 35876, 36600, 37280, 37943
};

GGWaveESP32::GGWaveESP32() {
}

void GGWaveESP32::begin() {
    // Initialization (if needed)
}

bool GGWaveESP32::modulateData(const char* data, size_t length, int16_t* audioBuffer, size_t* audioLength) {
    if (!data || !audioBuffer || !audioLength) return false;
    
    const size_t samplesPerBit = GGWAVE_SAMPLE_RATE / 100; // 10ms per bit
    *audioLength = length * 8 * samplesPerBit; // 8 bits per byte
    
    size_t currentSample = 0;
    
    // For each byte in the data
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = data[i];
        
        // For each bit in the byte
        for (int bit = 0; bit < 8; bit++) {
            // Determine frequency based on bit value
            float freq = GGWAVE_TX_FREQ_START + ((byte >> bit) & 0x01) * GGWAVE_TX_FREQ_STEP;
            
            // Generate tone for this bit
            generateTone(audioBuffer + currentSample, samplesPerBit, freq, 0.5f);
            currentSample += samplesPerBit;
        }
    }
    
    return true;
}

void GGWaveESP32::generateTone(int16_t* buffer, size_t length, float frequency, float amplitude) {
    const float omega = 2.0f * PI * frequency / GGWAVE_SAMPLE_RATE;
    
    for (size_t i = 0; i < length; i++) {
        // Use sine table for faster computation
        float phase = fmod(omega * i, 2.0f * PI);
        uint8_t tableIndex = (uint8_t)(phase * 256.0f / (2.0f * PI));
        buffer[i] = amplitude * sinTable[tableIndex];
    }
}

bool GGWaveESP32::demodulateAudio(const int16_t* audioBuffer, size_t length, char* data, size_t* dataLength) {
    if (!audioBuffer || !data || !dataLength) return false;
    
    const size_t samplesPerBit = GGWAVE_SAMPLE_RATE / 100;
    const size_t fftSize = GGWAVE_SAMPLE_SIZE;
    
    float* fftBuffer = new float[fftSize];
    size_t bytesDecoded = 0;
    uint8_t currentByte = 0;
    int bitCount = 0;
    
    // Process audio buffer in blocks
    for (size_t i = 0; i < length; i += samplesPerBit) {
        // Perform FFT for this block
        performFFT(audioBuffer + i, fftBuffer, fftSize);
        
        // Find dominant frequency
        float maxMagnitude = 0;
        int maxIndex = 0;
        
        for (int j = 0; j < GGWAVE_NUM_FREQUENCIES; j++) {
            float freq = GGWAVE_TX_FREQ_START + j * GGWAVE_TX_FREQ_STEP;
            int binIndex = (int)(freq * fftSize / GGWAVE_SAMPLE_RATE);
            
            if (fftBuffer[binIndex] > maxMagnitude) {
                maxMagnitude = fftBuffer[binIndex];
                maxIndex = j;
            }
        }
        
        // Decode the bit
        currentByte |= (maxIndex > 0) << bitCount;
        bitCount++;
        
        // If we have a complete byte
        if (bitCount == 8) {
            data[bytesDecoded++] = currentByte;
            currentByte = 0;
            bitCount = 0;
        }
    }
    
    *dataLength = bytesDecoded;
    delete[] fftBuffer;
    return true;
}

void GGWaveESP32::performFFT(const int16_t* input, float* output, size_t length) {
    // Simple DFT implementation (to be replaced with optimized FFT later)
    for (size_t k = 0; k < length/2; k++) {
        float real = 0;
        float imag = 0;
        
        for (size_t n = 0; n < length; n++) {
            float angle = -2.0f * PI * k * n / length;
            real += input[n] * cos(angle);
            imag += input[n] * sin(angle);
        }
        
        output[k] = sqrt(real*real + imag*imag) / length;
    }
} 