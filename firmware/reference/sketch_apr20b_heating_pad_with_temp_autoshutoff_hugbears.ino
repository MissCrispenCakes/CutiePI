/* safety test:
1) Open the Serial Monitor: Watch the numbers for a minute before you put the sensor inside the toy.
2) Verify the Direction: Hold the sensor. If the temperature decreases when you touch it, swap the 5V and GND wires 
3) The "Squeeze" Test: Once it's in the toy, give it a big hug. Make sure the temperature reading doesn't jump wildly (which would mean a loose wire or a short circuit).
*/
// Function to calculate temperature from Keyes KY-013 thermistor
#include <math.h> // Required for the log() function

// Pin Definitions
const int heaterPin = 3;   // PWMN Pin MOSFET/Transistor Gate
const int touchPin = 2;    // Digital Input Touch sensor (OSEPP-TOUCH-01)
const int sensorPin = A0;  // Temperature Sensor analog input (Keyes KY-013 thermistor)
const int ledPin = 13;     // Arduino built-in LED for status

// Safety Parameters (in Celsius)
const float MAX_SAFE_TEMP = 48.0; // Hard cutoff for safety
const float TARGET_TEMP = 42.0;   // Comfort level goal
const float HYSTERESIS = 2.0;     // Buffer to prevent rapid toggling

bool isOn = false;

void setup() {
  pinMode(heaterPin, OUTPUT);
  pinMode(touchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float currentTemp = getTemp();

// 1. Touch Toggle Logic
  if (digitalRead(touchPin) == HIGH) {
    isOn = !isOn;
    digitalWrite(ledPin, isOn);
    delay(500); // Debounce
  }

  // 2. Critical Safety Checks
  if (currentTemp >= MAX_SAFE_TEMP || currentTemp < 0) {
    emergencyShutdown();
  }

  // 3. Smart Heating Logic
  if (isOn) {
    if (currentTemp < (TARGET_TEMP - HYSTERESIS)) {
      // "Soft-Start": Pulse the power briefly to save battery stress
      analogWrite(heaterPin, 255); 
    } 
    else if (currentTemp >= TARGET_TEMP) {
      analogWrite(heaterPin, 0); 
    }
  } else {
    analogWrite(heaterPin, 0);
  }


  // Serial Monitor for debugging
  Serial.print("Current: "); Serial.print(currentTemp);
  Serial.print(" C | Heater: "); Serial.println(isOn ? "ON" : "OFF");

  delay(1000); // Check temperature every second
}


float getTemp() {
  int RawADC = analogRead(sensorPin);
  if (RawADC == 0) return -1.0; 
  double Temp = log(10000.0 * ((1024.0 / RawADC - 1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
  return (float)Temp - 273.15;
}

void emergencyShutdown() {
  analogWrite(heaterPin, 0);
  while(1) {
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(100);
  }
}
