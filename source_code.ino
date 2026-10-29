
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 6, 7, 8);

// Existing pin connections
const int TEMP_PIN = A0;
const int GAS_PIN = A1;

const int GREEN_LED = 2;
const int YELLOW_LED = 3;
const int RED_LED = 4;
const int BUZZER = 9;

// Demo thresholds
const float WARNING_TEMP = 35.0;
const float FIRE_TEMP = 50.0;

const int GAS_WARNING = 550;
const int GAS_FIRE = 700;

// Rate-of-rise threshold in degrees C per minute.
// Tune only after observing your Tinkercad sensor behavior.
const float RAPID_RISE_C_PER_MIN = 5.0;

// Gas smoothing
const int GAS_SAMPLES = 5;
const int GAS_HYSTERESIS = 30;

// States
const int SAFE = 0;
const int WARNING = 1;
const int FIRE_ALERT = 2;

int currentState = SAFE;
int filteredGas = 0;

float previousTemp = 0.0;
float temperatureRiseRate = 0.0;

unsigned long previousTempTime = 0;
unsigned long lastAlarmUpdate = 0;
unsigned long lastDisplayUpdate = 0;

int alarmStep = 0;
bool alarmOn = false;

// Read and average several gas readings
int readGasFiltered() {
  long total = 0;

  for (int i = 0; i < GAS_SAMPLES; i++) {
    total += analogRead(GAS_PIN);
  }

  return total / GAS_SAMPLES;
}

// Update LED outputs according to state
void updateLEDs(int state) {
  // Active-HIGH: HIGH = ON
  digitalWrite(GREEN_LED, state == SAFE ? HIGH : LOW);
  digitalWrite(YELLOW_LED, state == WARNING ? HIGH : LOW);
  digitalWrite(RED_LED, state == FIRE_ALERT ? HIGH : LOW);
}

// Non-blocking alarm patterns
void updateAlarm(unsigned long now) {
  unsigned long interval;
  bool shouldSound;

  if (currentState == SAFE) {
    noTone(BUZZER);
    alarmOn = false;
    alarmStep = 0;
    return;
  }

  if (currentState == WARNING) {
    // One short beep every second
    interval = (alarmStep == 0) ? 150 : 850;

    if (now - lastAlarmUpdate >= interval) {
      lastAlarmUpdate = now;
      alarmStep = (alarmStep + 1) % 2;
      alarmOn = (alarmStep == 1);
    }

    shouldSound = alarmOn;
  } else {
    // Three rapid beeps, then a longer pause
    const unsigned long pattern[] = {
      150, 150, 150, 150, 150, 650
    };

    if (now - lastAlarmUpdate >= pattern[alarmStep]) {
      lastAlarmUpdate = now;
      alarmStep = (alarmStep + 1) % 6;
      alarmOn = (alarmStep == 0 ||
                 alarmStep == 2 ||
                 alarmStep == 4);
    }

    shouldSound = alarmOn;
  }

  if (shouldSound) {
    tone(BUZZER, currentState == FIRE_ALERT ? 1500 : 700);
  } else {
    noTone(BUZZER);
  }
}

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("SMART FIRE V2");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  updateLEDs(SAFE);
  noTone(BUZZER);

  delay(1500);
  lcd.clear();

  filteredGas = readGasFiltered();

  int rawTemp = analogRead(TEMP_PIN);
  float voltage = rawTemp * (5.0 / 1023.0);
  previousTemp = (voltage - 0.5) * 100.0;

  previousTempTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Read TMP36
  int rawTemp = analogRead(TEMP_PIN);
  float voltage = rawTemp * (5.0 / 1023.0);
  float temperatureC = (voltage - 0.5) * 100.0;

  // Smooth gas readings using a moving average
  int newGas = readGasFiltered();

  // Exponential smoothing: 70% previous, 30% new
  filteredGas = (filteredGas * 7 + newGas * 3) / 10;

  // Calculate temperature rise rate once per second
  unsigned long elapsed = now - previousTempTime;

  if (elapsed >= 1000) {
    temperatureRiseRate =
      (temperatureC - previousTemp) * 60000.0 / elapsed;

    previousTemp = temperatureC;
    previousTempTime = now;
  }

  // Determine alert using hysteresis
  int nextState = currentState;

  if (temperatureC >= FIRE_TEMP ||
      filteredGas >= GAS_FIRE ||
      temperatureRiseRate >= RAPID_RISE_C_PER_MIN) {
    nextState = FIRE_ALERT;
  }
  else if (temperatureC >= WARNING_TEMP ||
           filteredGas >= GAS_WARNING) {
    nextState = WARNING;
  }
  else if (temperatureC < WARNING_TEMP - 1.0 &&
           filteredGas < GAS_WARNING - GAS_HYSTERESIS) {
    nextState = SAFE;
  }

  // Update alarm phase when state changes
  if (nextState != currentState) {
    currentState = nextState;
    alarmStep = 0;
    alarmOn = false;
    lastAlarmUpdate = now;
  }

  updateLEDs(currentState);
  updateAlarm(now);

  // Refresh LCD approximately twice per second
  if (now - lastDisplayUpdate >= 500) {
    lastDisplayUpdate = now;

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temperatureC, 1);
    lcd.write(byte(223));
    lcd.print("C G:");
    lcd.print(filteredGas);
    lcd.print("  ");

    lcd.setCursor(0, 1);

    if (currentState == SAFE) {
      lcd.print("STATE: SAFE     ");
    } else if (currentState == WARNING) {
      lcd.print("STATE: WARNING  ");
    } else {
      lcd.print("STATE: FIRE ALERT");
    }
  }

  // Diagnostic output
  Serial.print("Temp:");
  Serial.print(temperatureC, 1);
  Serial.print(" C | Gas:");
  Serial.print(filteredGas);
  Serial.print(" | Rise:");
  Serial.print(temperatureRiseRate, 1);
  Serial.print(" C/min | State:");

  if (currentState == SAFE) {
    Serial.println("SAFE");
  } else if (currentState == WARNING) {
    Serial.println("WARNING");
  } else {
    Serial.println("FIRE ALERT");
  }
}
