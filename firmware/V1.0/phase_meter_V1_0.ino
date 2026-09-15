#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte PIN_VOLTAGE = 2;
const byte PIN_CURRENT = 3;

const unsigned long SIGNAL_TIMEOUT = 1000000UL;

volatile unsigned long voltageTime = 0;
volatile unsigned long currentTime = 0;
volatile unsigned long currentPeriod = 0;

// Грецька буква φ
byte phi[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B10101,
  B01110,
  B00100,
  B00100
};

void voltageISR() {
  voltageTime = micros();
}

void currentISR() {
  unsigned long now = micros();

  if (currentTime != 0) {
    currentPeriod = now - currentTime;
  }

  currentTime = now;
}

void setup() {

  pinMode(PIN_VOLTAGE, INPUT);
  pinMode(PIN_CURRENT, INPUT);

  lcd.init();
  lcd.backlight();

  // Створення символу φ
  lcd.createChar(0, phi);

  // -----------------------------
  // Екран запуску
  // -----------------------------

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("PHASE METER");

  lcd.setCursor(0, 1);
  lcd.print("V1.0");

  delay(2000);

  // -----------------------------
  // Запуск переривань
  // -----------------------------

  attachInterrupt(
    digitalPinToInterrupt(PIN_VOLTAGE),
    voltageISR,
    RISING
  );

  attachInterrupt(
    digitalPinToInterrupt(PIN_CURRENT),
    currentISR,
    RISING
  );

  lcd.clear();
}

void loop() {

  unsigned long vTime;
  unsigned long cTime;
  unsigned long period;

  noInterrupts();

  vTime = voltageTime;
  cTime = currentTime;
  period = currentPeriod;

  interrupts();

  unsigned long now = micros();

  // -----------------------------
  // Немає сигналу струму
  // -----------------------------

  if (cTime == 0 ||
      (unsigned long)(now - cTime) > SIGNAL_TIMEOUT ||
      period == 0) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("FI:0.0");
    lcd.write((char)223);
    lcd.print("     ");

    lcd.setCursor(0, 1);
    lcd.print("F:0.0Hz ");

    lcd.write(byte(0));
    lcd.print(":1.000");

    delay(200);

    return;
  }

  // -----------------------------
  // Частота струму
  // -----------------------------

  float frequency = 1000000.0 / period;

  // -----------------------------
  // Різниця часу D2 - D3
  // -----------------------------

  long delta = (long)(cTime - vTime);

  while (delta > (long)(period / 2)) {
    delta -= period;
  }

  while (delta < -(long)(period / 2)) {
    delta += period;
  }

  // -----------------------------
  // Кут
  // -----------------------------

  float angle = 360.0 * delta / period;

  if (angle > 180.0)
    angle = 180.0;

  if (angle < -180.0)
    angle = -180.0;

  // -----------------------------
  // Косинус φ
  // -----------------------------

  float cosPhi = cos(angle * PI / 180.0);

  // -----------------------------
  // L / C
  // -----------------------------

  char type;

  if (angle > 0.5) {
    type = 'L';
  }
  else if (angle < -0.5) {
    type = 'C';
  }
  else {
    type = ' ';
  }

  // -----------------------------
  // Рядок 1
  // -----------------------------

  lcd.setCursor(0, 0);

  lcd.print("FI:");

  if (angle >= 0)
    lcd.print("+");

  lcd.print(angle, 1);

  lcd.write((char)223);

  lcd.print(" ");
  lcd.print(type);

  lcd.print("     ");

  // -----------------------------
  // Рядок 2
  // -----------------------------

  lcd.setCursor(0, 1);

  lcd.print("F:");
  lcd.print(frequency, 1);
  lcd.print("Hz ");

  // φ
  lcd.write(byte(0));

  lcd.print(":");
  lcd.print(cosPhi, 3);

  delay(200);
}
