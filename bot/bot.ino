#include <SoftwareSerial.h>

#define AIN1 11
#define AIN2 10
#define BIN1 9
#define BIN2 8
#define RX 12
#define TX 13
#define speedControl 7
#define BAUD 9600

enum DIRECTION {
  FORWARD,
  LEFT,
  REVERSE,
  RIGHT,
  STOP
};

DIRECTION Direction;
SoftwareSerial BTSerial(RX, TX);
unsigned char rawByte;
unsigned char lastByte = 0xAF;
unsigned int PWM = 90;


void setup() {
  Serial.begin(BAUD);

  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  BTSerial.begin(BAUD);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(speedControl, OUTPUT);

  analogWrite(speedControl, PWM);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  
  
}


void loop() {
  if (BTSerial.available()) {
    Serial.println(BTSerial.read());
    Direction = getDirection();
    drive(Direction);
  }
}


DIRECTION getDirection() {
  rawByte = BTSerial.read();
  if(rawByte != lastByte){
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
  lastByte = rawByte;
  }
  
}


void drive(DIRECTION direction) {
  switch (direction) {
    case FORWARD:
      {
        analogWrite(speedControl, PWM);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
        break;
      }
    case LEFT:
      {
        analogWrite(speedControl, PWM);
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, LOW);
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        break;
      }
    case REVERSE:
      {
        analogWrite(speedControl, PWM);
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
        break;
      }
    case RIGHT:
      {
        analogWrite(speedControl, PWM);
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
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