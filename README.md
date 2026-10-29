# Smart Fire Detection and Emergency Warning System

An Arduino Uno–based fire-warning prototype that combines a gas sensor and a TMP36 temperature sensor to monitor potentially hazardous conditions. The system uses a 16×2 LCD to display information and LEDs plus a piezo buzzer to provide visual and audible warnings.

> **Live working demo:** [Open the project in Tinkercad Circuits](https://www.tinkercad.com/things/jabf8oZWK4T-smart-fire-detection-and-emergency-warning-system?sharecode=QtIlasAQOnUIjuNDHBw93mRWHET3NtbR4KsRob6hnmU)

## Circuit Diagram

![Circuit diagram for the Smart Fire Detection and Emergency Warning System](<circuit_diagram.png>)

The diagram shows the Arduino Uno, gas sensor, TMP36 temperature sensor, 16×2 LCD, potentiometer, three status LEDs, resistors, piezo buzzer, and breadboard.

## Components Used

| Component | Quantity | Purpose |
|---|---:|---|
| Arduino Uno R3 | 1 | Reads sensor inputs and controls the display and alerts |
| Small breadboard | 1 | Prototyping and circuit connections |
| Gas sensor | 1 | Detects changes in gas/smoke-related sensor readings |
| TMP36 temperature sensor | 1 | Measures ambient temperature |
| 16×2 LCD | 1 | Displays readings and system status |
| 10 kΩ potentiometer | 1 | Adjusts the LCD contrast |
| Green LED | 1 | Visual status indicator |
| Yellow LED | 1 | Visual warning indicator |
| Red LED | 1 | Visual danger/alarm indicator |
| 220 Ω resistors | 4 | Current limiting for LEDs / circuit as wired |
| 10 kΩ resistor | 1 | Gas-sensor circuit component, as shown in the diagram |
| Piezo buzzer | 1 | Audible warning |

## How It Works

1. **Sensing:** The Arduino reads the gas-sensor output and the TMP36 temperature signal.
2. **Evaluation:** The readings can be compared against thresholds defined in the Arduino program.
3. **Display:** The 16×2 LCD presents sensor readings and/or the current warning status.
4. **Visual warning:** The green, yellow, and red LEDs provide different status indications.
5. **Audible warning:** The piezo buzzer sounds when the programmed alarm condition is met.

The exact threshold values, LCD messages, LED states, and buzzer behaviour depend on the Arduino code used in the Tinkercad circuit. Calibrate and verify these values before interpreting the prototype's output.

## Run the Live Simulation

1. Open the [Tinkercad project](https://www.tinkercad.com/things/jabf8oZWK4T-smart-fire-detection-and-emergency-warning-system?sharecode=QtIlasAQOnUIjuNDHBw93mRWHET3NtbR4KsRob6hnmU).
2. Sign in to Tinkercad if prompted.
3. Open the circuit and select **Start Simulation**.
4. Observe the LCD, LEDs, and buzzer.
5. If the circuit's code or sensor controls allow it, change the sensor inputs to test normal and alarm conditions.

## Repository Structure

```text
.
├── README.md
└── circuit-diagram.png
└── components.csv
└── source_code.ino
```

## Limitations and Safety

- This is an educational prototype/simulation, **not a certified fire alarm**.
- A generic gas sensor does not uniquely identify smoke or fire. Sensor readings can be affected by different gases, environmental conditions, warm-up time, and calibration.
- The TMP36 measures temperature; it does not directly detect flames.
- Thresholds must be selected and tested for the specific sensor and environment.
- Do not rely on this project for life-safety protection. Use certified smoke/fire alarms in real buildings.
- For a physical build, verify the sensor pinout and component datasheets, keep wiring secure, and test the buzzer and indicators under controlled conditions.

## Future Improvements

- Show live temperature and gas-sensor values on the LCD.
- Add clearly defined normal, warning, and danger thresholds with hysteresis to prevent rapid state switching.
- Add sensor warm-up handling, filtering, and fault detection.
- Document the exact Arduino pin mapping and the alarm thresholds in the source code.
- Test the system against repeatable simulated inputs and record the expected output for each case.
- Add a test/reset control and a startup self-check.

## Technologies

- Arduino Uno R3
- Arduino C/C++
- Sensors and embedded systems
- Tinkercad Circuits

## Demo

**[Launch the live Tinkercad simulation →](https://www.tinkercad.com/things/jabf8oZWK4T-smart-fire-detection-and-emergency-warning-system?sharecode=QtIlasAQOnUIjuNDHBw93mRWHET3NtbR4KsRob6hnmU)**
