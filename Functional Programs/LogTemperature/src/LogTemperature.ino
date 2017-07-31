/*
 * Project LogTemperature
 * Description: A functional test for the TMP36 Analog Temperature Sensor
 * Author: Trevor Trouchon
 * Date: 7-30-17
 */

SYSTEM_MODE(SEMI_AUTOMATIC);  // Prevent code-blocking cellular startup, since we're using a serial connection to test.

void setup() {
  Serial.begin();
}

void loop() {
  int readAnalogValue = analogRead(A0);
  double readVoltage = 3.3 * ((double)readAnalogValue / 4095.0);

  double temperature = 100 * readVoltage - 50;
  Serial.println("Temperature: " + String(temperature));
  delay(2000);
}
