#include <Servo.h>

#define PIR_PIN     11
#define BUZZER_PIN  10
#define SERVO_PIN   9
#define MOTOR_PIN   8
#define LED_PIN     12

#define TEMP_PIN    A1
#define SOIL_PIN    A2
#define POT_PIN     A0

Servo myServo;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(0);

  Serial.begin(9600);
}

void loop() {

  // ===== Temperature Sensor =====
  int tempRaw = analogRead(TEMP_PIN);
  float voltage = tempRaw * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100.0;   // TMP36

  // Servo moves when temperature rises
  if (temperature > 30) {
    myServo.write(90);
  } else {
    myServo.write(0);
  }

  // ===== Potentiometer Controls LED =====
  int potValue = analogRead(POT_PIN);

  // Since LED is on D12 (not PWM), use ON/OFF control
  if (potValue > 512) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // ===== PIR Controls Buzzer =====
  int pirState = digitalRead(PIR_PIN);

  if (pirState == HIGH) {
    tone(BUZZER_PIN, 1000);
  } else {
    noTone(BUZZER_PIN);
  }

  // ===== Soil Moisture Controls Motor =====
  int soilValue = analogRead(SOIL_PIN);

  // Dry soil -> motor ON
  if (soilValue < 500) {
    digitalWrite(MOTOR_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
  }

  // Serial Monitor Output
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C  ");

  Serial.print("Pot: ");
  Serial.print(potValue);

  Serial.print("  PIR: ");
  Serial.print(pirState);

  Serial.print("  Soil: ");
  Serial.println(soilValue);

  delay(200);
}