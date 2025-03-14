#include <ESP32GibberLink.h>

// Pin definitions
#define MIC_PIN     34  // I2S microphone data pin (not used in sender)
#define SPEAKER_PIN 25  // DAC1 (GPIO25) for audio output

ESP32GibberLink gibberlink;

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 GibberLink Sender Test");
    
    // Initialize the GibberLink library
    if (!gibberlink.begin(MIC_PIN, SPEAKER_PIN)) {
        Serial.println("Initialization error!");
        while (1) {
            delay(1000);
        }
    }
    
    Serial.println("Ready to send data...");
    Serial.println("Enter text and press Enter to send.");
}

void loop() {
    if (Serial.available()) {
        // Read input until newline
        String input = Serial.readStringUntil('\n');
        
        // Send the data
        if (gibberlink.sendData(input.c_str(), input.length())) {
            Serial.println("Data sent!");
        } else {
            Serial.println("Error sending data!");
        }
    }
    
    delay(100);
} 