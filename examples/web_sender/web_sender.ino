#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32GibberLink.h>

// WiFi Settings
const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";

// Select your audio output type
#define AUDIO_OUTPUT_DAC      1  // Simple speaker connected to DAC1 (GPIO25)
#define AUDIO_OUTPUT_I2S      2  // MAX98357A I2S Amplifier
#define AUDIO_OUTPUT_TYPE     AUDIO_OUTPUT_DAC  // <- Change this to select output type

// Pin Definitions
#define MIC_PIN     34    // I2S Microphone data pin

#if AUDIO_OUTPUT_TYPE == AUDIO_OUTPUT_DAC
    #define SPEAKER_PIN 25  // DAC1 (GPIO25) for direct speaker connection
#else
    // I2S Amplifier Pins
    #define I2S_BCLK    26  // Bit Clock
    #define I2S_LRC     25  // Left/Right Clock
    #define I2S_DOUT    22  // Data Out
#endif

// Webserver on port 80
AsyncWebServer server(80);
ESP32GibberLink gibberlink;

// Callback function for received data
void onDataReceived(const char* data, size_t length) {
    Serial.println("Data received:");
    Serial.write((uint8_t*)data, length);
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    
    Serial.println("\nESP32 GibberLink Web Sender");
    Serial.println("============================");
    
#if AUDIO_OUTPUT_TYPE == AUDIO_OUTPUT_DAC
    Serial.println("Audio Output: DAC (Direct Speaker)");
    Serial.println("Connect an 8Ω speaker or piezo as follows:");
    Serial.println("- Positive -> GPIO25 (via 100Ω resistor)");
    Serial.println("- Negative -> GND");
#else
    Serial.println("Audio Output: I2S (MAX98357A Amplifier)");
    Serial.println("Connect the MAX98357A as follows:");
    Serial.println("- BCLK -> GPIO26");
    Serial.println("- LRC  -> GPIO25");
    Serial.println("- DIN  -> GPIO22");
    Serial.println("- VIN  -> 3.3V");
    Serial.println("- GND  -> GND");
#endif
    
    // Initialize SPIFFS
    if(!SPIFFS.begin(true)) {
        Serial.println("Error mounting SPIFFS");
        return;
    }
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Initialize the GibberLink library
#if AUDIO_OUTPUT_TYPE == AUDIO_OUTPUT_DAC
    if (!gibberlink.begin(MIC_PIN, SPEAKER_PIN)) {
#else
    if (!gibberlink.beginI2S(MIC_PIN, I2S_BCLK, I2S_LRC, I2S_DOUT)) {
#endif
        Serial.println("Error initializing GibberLink!");
        return;
    }
    
    // Set the callback function for received data
    gibberlink.setDataCallback(onDataReceived);
    
    // Start recording
    if (!gibberlink.startRecording()) {
        Serial.println("Error starting recording!");
        return;
    }
    
    // Webserver routes
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    
    // Start the webserver
    server.begin();
    
    Serial.println("\nWebserver started!");
    Serial.println("Open http://" + WiFi.localIP().toString() + " in your browser");
    Serial.println("\nReady to receive data...");
}

void loop() {
    // Main processing happens in the interrupt routine
    delay(1000);
} 