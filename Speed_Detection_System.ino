#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int ir1 = 2;
const int ir2 = 3;
const int redLED = 4;
const int greenLED = 5;
const int buzzer = 6;

float distance_cm = 10.0;
float speedLimit_cmps = 30.0;

unsigned long startTime, endTime;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Change to 0x3F if needed

void setup() {
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Speed Detector");
}

void loop() {
  if (digitalRead(ir1) == LOW) {
    startTime = millis();

    while (digitalRead(ir2) == HIGH); // Wait until object reaches second sensor

    endTime = millis();
    float time_sec = (endTime - startTime) / 1000.0;

    float speed_cmps = distance_cm / time_sec;
    float speed_mps = speed_cmps / 100.0;
    float speed_kmph = speed_mps * 3.6;

    Serial.print("Speed: ");
    Serial.print(speed_cmps, 2);
    Serial.print(" cm/s | ");
    Serial.print(speed_kmph, 2);
    Serial.println(" km/h");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("S:");
    lcd.print(speed_cmps, 0);
    lcd.print("cm/s ");
    lcd.print(speed_kmph, 1);
    lcd.print("km/h");

    lcd.setCursor(0, 1);

    if (speed_cmps > speedLimit_cmps) {
      digitalWrite(redLED, HIGH);
      digitalWrite(greenLED, LOW);
      digitalWrite(buzzer, HIGH); // Buzzer ON
      lcd.print("Overspeed Alert!");
      delay(2000);

      // Turn off red LED and buzzer, turn on green
      digitalWrite(redLED, LOW);
      digitalWrite(buzzer, LOW);  // Buzzer OFF
      digitalWrite(greenLED, HIGH);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Speed:");
      lcd.print(speed_cmps, 0);
      lcd.print("cm/s OK");
      lcd.setCursor(0, 1);
      lcd.print("Now Safe Speed");
    } else {
      digitalWrite(redLED, LOW);
      digitalWrite(buzzer, LOW);
      digitalWrite(greenLED, HIGH);
      lcd.print("Speed Normal   ");
    }

    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed Detector");
  }
}
