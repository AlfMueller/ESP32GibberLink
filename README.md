# ESP32 GibberLink

Eine ESP32-Portierung der [GibberLink](https://github.com/PennyroyalTea/gibberlink) Bibliothek für die Übertragung von Daten über Audiowellen.

## Status

Dieses Projekt befindet sich noch in der Entwicklung. Die grundlegende Funktionalität ist implementiert, aber einige Features sind möglicherweise noch unvollständig oder können sich ändern.

## Features

- Übertragung von Daten über Audiowellen
- Web-Interface zum Senden von Nachrichten
- Unterstützung für direkte Lautsprecherverbindung (DAC) und I2S-Verstärker
- Kompatibel mit allen ASCII-Zeichen
- Einfache Integration in bestehende Projekte
- Web-basierter Sender enthalten

## Hardware-Anforderungen

### Basis-Setup (DAC-Ausgang)
- ESP32 Entwicklungsboard
- I2S MEMS Mikrofon (z.B. INMP441)
- 8Ω Lautsprecher oder Piezo-Summer
- 100Ω Widerstand

### Erweitertes Setup (I2S-Ausgang)
- ESP32 Entwicklungsboard
- I2S MEMS Mikrofon (z.B. INMP441)
- MAX98357A I2S-Verstärker
- Lautsprecher (4Ω oder 8Ω)

## Pin-Verbindungen

### Mikrofon (INMP441)
```
ESP32    INMP441
3.3V --- VDD
GND  --- GND
GPIO34 - SD (Daten)
GND  --- L/R
GPIO25 - WS (Word Select)
GPIO26 - SCK (Serial Clock)
```

### Lautsprecher-Optionen

#### 1. Direkte DAC-Verbindung
```
ESP32              Lautsprecher
GPIO25 --- 100Ω --- Plus
GND   ------------ Minus
```

#### 2. I2S-Verstärker (MAX98357A)
```
ESP32         MAX98357A
GPIO26 ------ BCLK
GPIO25 ------ LRC
GPIO22 ------ DIN
3.3V  ------ VIN
GND   ------ GND
```

## Installation

1. Laden Sie dieses Repository als ZIP herunter
2. In der Arduino IDE: Sketch -> Bibliothek einbinden -> .ZIP Bibliothek hinzufügen
3. Wählen Sie die heruntergeladene ZIP-Datei aus

## Verwendung

### Basis-Beispiel
```cpp
#include <ESP32GibberLink.h>

ESP32GibberLink gibberlink;

void onDataReceived(const char* data, size_t length) {
    Serial.println("Empfangen: ");
    Serial.write((uint8_t*)data, length);
}

void setup() {
    // Initialisiere mit DAC-Ausgang
    gibberlink.begin(MIC_PIN, SPEAKER_PIN);
    // Oder initialisiere mit I2S-Ausgang
    // gibberlink.beginI2S(MIC_PIN, I2S_BCLK, I2S_LRC, I2S_DOUT);
    
    gibberlink.setDataCallback(onDataReceived);
    gibberlink.startRecording();
}
```

### Web-Interface

Die Bibliothek enthält ein Web-Interface-Beispiel zum Senden von Nachrichten aus jedem Browser:

1. Laden Sie das `web_sender` Beispiel hoch
2. Verbinden Sie sich mit dem WiFi-Netzwerk des ESP32
3. Öffnen Sie die angezeigte IP-Adresse in Ihrem Browser
4. Geben Sie Text ein und klicken Sie auf "Senden"

## Konfiguration

In `web_sender.ino` können Sie den Audio-Ausgangstyp wählen:
```cpp
// Wählen Sie Ihren Audio-Ausgangstyp
#define AUDIO_OUTPUT_TYPE  AUDIO_OUTPUT_DAC  // Einfacher Lautsprecher
// oder
#define AUDIO_OUTPUT_TYPE  AUDIO_OUTPUT_I2S  // I2S-Verstärker
```

## Protokoll-Details

Das Audio-Protokoll verwendet:
- Abtastrate: 16kHz
- Bittiefe: 16-bit
- Frequenzbereich: 1000Hz - 2600Hz
- Bitdauer: 10ms
- Fehlererkennung: Basis-Paketvalidierung

## Einschränkungen

- Maximale Nachrichtenlänge: 256 Bytes
- Übertragungsgeschwindigkeit: ~100 Bytes/Sekunde
- Reichweite abhängig von Lautstärke und Umgebungsgeräuschen
- Beste Ergebnisse in ruhiger Umgebung

## Mitwirken

1. Forken Sie das Repository
2. Erstellen Sie Ihren Feature-Branch
3. Committen Sie Ihre Änderungen
4. Pushen Sie zum Branch
5. Erstellen Sie einen Pull Request

## Lizenz

Dieses Projekt ist unter der MIT-Lizenz lizenziert - siehe die LICENSE-Datei für Details.

## Danksagungen

- Original [GibberLink Projekt](https://github.com/PennyroyalTea/gibberlink)
- [ggwave Bibliothek](https://github.com/ggerganov/ggwave) von Georgi Gerganov
- ESP32 Arduino Core Entwickler
