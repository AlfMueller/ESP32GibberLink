#include "ESP32GibberLink.h"

ESP32GibberLink::ESP32GibberLink() : _callback(nullptr), _recordingTask(nullptr) {
    _audioBufferSize = BUFFER_SIZE;
    _audioBuffer = new int16_t[_audioBufferSize];
    _outputType = AUDIO_OUTPUT_DAC;
}

ESP32GibberLink::~ESP32GibberLink() {
    if (_audioBuffer) {
        delete[] _audioBuffer;
    }
    if (_recordingTask) {
        vTaskDelete(_recordingTask);
    }
    i2s_driver_uninstall(I2S_NUM_0);
}

bool ESP32GibberLink::begin(int micPin, int speakerPin) {
    _micPin = micPin;
    _speakerPin = speakerPin;
    _outputType = AUDIO_OUTPUT_DAC;
    
    // I2S configuration for microphone
    i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };
    
    // I2S pin configuration
    i2s_pins = {
        .bck_io_num = 26,     // BCK (Bit Clock)
        .ws_io_num = 25,      // LRCK (Word Select)
        .data_out_num = _speakerPin,
        .data_in_num = _micPin
    };
    
    // Install I2S driver
    esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        return false;
    }
    
    // Set I2S pins
    err = i2s_set_pin(I2S_NUM_0, &i2s_pins);
    if (err != ESP_OK) {
        return false;
    }
    
    // Initialize GGWave
    _ggwave.begin();
    
    return true;
}

bool ESP32GibberLink::beginI2S(int micPin, int bclkPin, int wsPin, int dataPin) {
    _micPin = micPin;
    _bclkPin = bclkPin;
    _wsPin = wsPin;
    _dataPin = dataPin;
    _outputType = AUDIO_OUTPUT_I2S;
    
    // I2S configuration for microphone and amplifier
    i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };
    
    // I2S pin configuration
    i2s_pins = {
        .bck_io_num = _bclkPin,
        .ws_io_num = _wsPin,
        .data_out_num = _dataPin,
        .data_in_num = _micPin
    };
    
    // Install I2S driver
    esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (err != ESP_OK) {
        return false;
    }
    
    // Set I2S pins
    err = i2s_set_pin(I2S_NUM_0, &i2s_pins);
    if (err != ESP_OK) {
        return false;
    }
    
    // Initialize GGWave
    _ggwave.begin();
    
    return true;
}

bool ESP32GibberLink::startRecording() {
    // Start I2S
    esp_err_t err = i2s_start(I2S_NUM_0);
    if (err != ESP_OK) {
        return false;
    }
    
    // Create task for continuous reading
    xTaskCreate(
        recordingTaskFunction,
        "recording_task",
        4096,
        this,
        1,
        &_recordingTask
    );
    
    return true;
}

void ESP32GibberLink::recordingTaskFunction(void* parameter) {
    ESP32GibberLink* instance = (ESP32GibberLink*)parameter;
    size_t bytesRead = 0;
    
    while (true) {
        // Read audio data from I2S
        i2s_read(I2S_NUM_0, instance->_audioBuffer, instance->_audioBufferSize * sizeof(int16_t), &bytesRead, portMAX_DELAY);
        
        if (bytesRead > 0) {
            instance->processAudioData(instance->_audioBuffer, bytesRead / sizeof(int16_t));
        }
        
        vTaskDelay(1); // Small delay for other tasks
    }
}

bool ESP32GibberLink::stopRecording() {
    if (_recordingTask != nullptr) {
        vTaskDelete(_recordingTask);
        _recordingTask = nullptr;
    }
    
    // Stop I2S
    esp_err_t err = i2s_stop(I2S_NUM_0);
    if (err != ESP_OK) {
        return false;
    }
    
    return true;
}

bool ESP32GibberLink::sendData(const char* data, size_t length) {
    int16_t* txBuffer;
    size_t txLength;
    
    // Modulate data to audio
    if (!_ggwave.modulateData(data, length, txBuffer, &txLength)) {
        return false;
    }
    
    // Play the audio buffer
    bool result = playAudioBuffer(txBuffer, txLength);
    
    delete[] txBuffer;
    return result;
}

bool ESP32GibberLink::playAudioBuffer(const int16_t* buffer, size_t length) {
    size_t bytesWritten = 0;
    
    if (_outputType == AUDIO_OUTPUT_DAC) {
        // For DAC output
        i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);  // GPIO25
    } else {
        // For I2S output
        i2s_set_clk(I2S_NUM_0, SAMPLE_RATE, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
    }
    
    // Write audio data
    esp_err_t err = i2s_write(I2S_NUM_0, buffer, length * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
    
    return (err == ESP_OK && bytesWritten == length * sizeof(int16_t));
}

void ESP32GibberLink::setDataCallback(DataCallback callback) {
    _callback = callback;
}

void ESP32GibberLink::processAudioData(int16_t* samples, size_t count) {
    char decodedData[256];
    size_t decodedLength;
    
    // Try to demodulate audio data
    if (_ggwave.demodulateAudio(samples, count, decodedData, &decodedLength)) {
        // If data was detected and a callback is registered
        if (decodedLength > 0 && _callback != nullptr) {
            _callback(decodedData, decodedLength);
        }
    }
} 