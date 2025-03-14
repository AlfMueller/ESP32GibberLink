#include <ESP32GibberLink.h>

// Pin definitions
#define MIC_PIN     34  // I2S microphone data pin
#define SPEAKER_PIN 25  // DAC1 (GPIO25) for audio output

ESP32GibberLink gibberlink;

// Callback function for received data
void onDataReceived(const char* data, size_t length) {
    Serial.println("Data received:");
    Serial.write((uint8_t*)data, length);
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 GibberLink Receiver Test");
    
    // Initialize the GibberLink library
    if (!gibberlink.begin(MIC_PIN, SPEAKER_PIN)) {
        Serial.println("Initialization error!");
        while (1) {
            delay(1000);
        }
    }
    
    // Set callback function for received data
    gibberlink.setDataCallback(onDataReceived);
    
    // Start recording
    if (!gibberlink.startRecording()) {
        Serial.println("Error starting recording!");
        while (1) {
            delay(1000);
        }
    }
    
    Serial.println("Ready to receive data...");
}

void loop() {
    // Main processing happens in the interrupt routine
    delay(1000);
} 