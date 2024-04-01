#include <SoftwareSerial.h>
#define AIN1 12
#define AIN2 11
#define BIN1 10
#define BIN2 9

#define RX 2
#define TX 3
#define BAUD 9600

enum DIRECTION {
  FORWARD,
  LEFT,
  REVERSE,
  RIGHT
};

DIRECTION Direction;
SoftwareSerial BTSerial(RX, TX);
unsigned char rawByte;


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
    Direction = getDirection();
    drive(Direction);
  }
}


DIRECTION getDirection() {
  rawByte = BTSerial.read();

  switch (rawByte) {
    case 0xAA:
      {
        Serial.println("forward");
        return FORWARD;
        break;
      }
    case 0xAB:
      {
        Serial.println("left");
        return LEFT;
        break;
      }
    case 0xAC:
      {
        Serial.println("reverse");
        return REVERSE;
        break;
      }
    case 0xAD:
      {
        Serial.println("right");
        return RIGHT;
        break;
      }
    case 0xAE:
      {
        Serial.println("stop");
        return STOP;
        break;
      }
    default:
      {
        Serial.println("Direction Err");
        break;
      }
  }
}


void drive(DIRECTION direction) {
  switch (direction) {
    case FORWARD:
      {
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        break;
      }
    case LEFT:
      {
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        break;
      }
    case REVERSE:
      {
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        break;
      }
    case RIGHT:
      {
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        break;
      }
      case STOP:
      {
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, LOW);
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
        break;
      }
    default: 
    {
      break;
    }
  }
}
