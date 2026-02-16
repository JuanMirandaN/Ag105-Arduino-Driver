# Ag105 Arduino Driver

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg) ![License](https://img.shields.io/badge/license-MIT-green.svg) ![Arduino](https://img.shields.io/badge/framework-Arduino-orange.svg)

An **unofficial**, Arduino driver for the **Silvertel Ag105** Solar MPPT Charger module. This library provides full I2C control over charging parameters, MPPT tracking, and status monitoring, making it easy to integrate high-power solar charging into your embedded projects.

> **Disclaimer:** This library is not affiliated with, endorsed by, or supported by Silvertel. Please refer to the official Silvertel Ag105 datasheet for hardware specifics.

## Features

* **Precise Control:** Dynamically set charging current (up to 2.5A) and battery voltage (up to 12.6V).
* **MPPT Management:** Configure Maximum Power Point Tracking voltage and recovery behaviors to optimize solar harvesting.
* **Telemetry & Monitoring:** Read real-time Battery Voltage, Charge Current, and Input (Solar) Voltage (Only available with Vin > 9V).
* **Built-in Debugging:** Includes formatted `ReportStatus()` and `ReportConfig()` functions for easy serial debugging in the field.
* **Platform Agnostic:** Works with any `TwoWire` (I2C) interface and `Stream` (Serial) port, supporting Arduino Uno, Mega, ESP32, STM32, and more.

## Installation

### Option 1: Arduino IDE Library Manager (Manual Zip)
1.  Download the latest release as a `.zip` file from the [Releases page](https://github.com/JuanMirandaN/Ag105-Arduino-Driver/releases).
2.  Open Arduino IDE.
3.  Go to **Sketch** -> **Include Library** -> **Add .ZIP Library...**
4.  Select the downloaded file.

### Option 2: PlatformIO
1.  Copy the `Ag105` folder into your project's `lib/` directory.
2.  Ensure `Wire` is initialized in your `main.cpp`.

## Wiring

The Ag105 communicates via I2C. Connect it to your microcontroller as follows:

| Ag105 Pin | Arduino Uno/Nano | ESP32 (Default) | Description |
| :--- | :--- | :--- | :--- |
| **SDA** | SDA | GPIO 21 | I2C Data |
| **SCL** | SCL | GPIO 22 | I2C Clock |
| **GND** | GND | GND | Ground (Common with Vin-) |
| **VCC** | 5V / 3.3V | 3.3V | Logic Power |

*Note: The Ag105 supports I2C logic levels from 3.3V to 5V. A 10k Ohm pull-up resistor on both SDA and SCL is advisable*

## Quick Start Example

Here is a simple example to initialize the charger and print status reports to the Serial Monitor.

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Ag105.h>

// Initialize Ag105 with Address 0x30, using Wire (I2C) and Serial for debug
Ag105 solar(0x30, &Wire, &Serial);


void setup() {
    Serial.begin(115200);
    Wire.begin();

    Serial.println("Initializing Ag105 Solar Charger...");

    // Configure Charger (Example: 1S Li-Ion at 500mA)
    // Returns 0 on success, 1 on failure
    if (solar.setBatteryVoltage(4.2) == 0) {
        Serial.println("Target Voltage set to 4.2V");
    } else {
        Serial.println("Failed to set Voltage!");
    }
    
    if (solar.setChargeCurrent(500.0) == 0) {
        Serial.println("Target Current set to 500mA");
    }

    // Print full configuration report once to verify settings
    solar.ReportConfig();
}

void loop() {
    // Print status report every 2 seconds
    solar.ReportStatus();
    delay(2000);
}
