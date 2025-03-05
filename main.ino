#define BLYNK_TEMPLATE_ID "TMPL34tleJCsy"
#define BLYNK_TEMPLATE_NAME "flow sensor"
#define BLYNK_AUTH_TOKEN "MJMHCa2DZVF3HGwTjfB4-iMvp7VwGfhF"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
const int flowSensorPin = 5;
const int turbiditySensorPin = 34;
const int tdsSensorPin = 37;
const int buzzersPin = 15;
volatile int pulseCount = 0;
float flowRate = 0.0;
unsigned long lastTime = 0;
const float calibrationFactor = 4.5;
char ssid[] = "desktop1";
char pass[] = "123456789";
char auth[] = BLYNK_AUTH_TOKEN;
void IRAM_ATTR pulseCounter() {
 pulseCount++;
}
void setup() {
 Serial.begin(115200);

 Blynk.begin(auth, ssid, pass);
 pinMode(flowSensorPin, INPUT_PULUP);
 attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALING);

 pinMode(turbiditySensorPin, INPUT);
 pinMode(tdsSensorPin, INPUT);
 pinMode(buzzerPin, OUTPUT);
 digitalWrite(buzzerPin, LOW);
}
void beepBuzzer(int beepCount) {
 for (int i = 0; i < beepCount; i++) {
 digitalWrite(buzzerPin, HIGH);
 delay(200);
 digitalWrite(buzzerPin, LOW);
 delay(200);
 }
}

void loop() {
 Blynk.run();
 if (millis() - lastTime >= 1000) {
 lastTime = millis();
 flowRate = (pulseCount / calibrationFactor) / 60.0;
 pulseCount = 0;
 if (flowRate > 0) {
 beepBuzzer(2);
 }
 Serial.print("Flow rate: ");
 Serial.print(flowRate);
 Serial.println(" L/min");
 Blynk.virtualWrite(V0, flowRate);
 int turbidityValue = analogRead(turbiditySensorPin);
 float turbidityVoltage = (turbidityValue / 4095.0) * 5.0;
 float turbidityCorrected = 5.0 - turbidityVoltage;
 Serial.print("Turbidity Corrected Value: ");
 Serial.println(turbidityCorrected);
 Blynk.virtualWrite(V1, turbidityCorrected);
 int tdsValue = analogRead(tdsSensorPin);
 float tdsVoltage = (tdsValue / 4095.0) * 5.0;
 Serial.print("TDS Voltage: ");
 Serial.println(tdsVoltage);
 Blynk.virtualWrite(V2, tdsVoltage);
 }
}
