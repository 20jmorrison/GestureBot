#include <SoftwareSerial.h>

#define AIN1 12
#define AIN2 11
#define BIN1 10
#define BIN2 9

#define RX 2
#define TX 3
#define BAUD 9600

SoftwareSerial BTSerial(RX, TX);

void setup() {
  Serial.begin(BAUD);
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  BTSerial.begin(BAUD);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
}

void loop() {
  if (BTSerial.available()) {
    Serial.println(BTSerial.read());
  }
}
