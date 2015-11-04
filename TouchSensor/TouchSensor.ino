#include <CapacitiveSensor.h>

CapacitiveSensor ySensor = CapacitiveSensor(4, A1);
CapacitiveSensor xSensor = CapacitiveSensor(5, A0);
CapacitiveSensor zSensor = CapacitiveSensor(2, A3);

uint8_t ARDUINO_COMM = 3;
uint8_t UP = 128*7;
uint8_t DOWN = 128*6;
uint8_t RIGHT = 128*5;
uint8_t LEFT = 128*4;
uint8_t FORWARD = 128*3;
uint8_t BACK = 128*2;

void setup() {
  // put your setup code here, to run once:
  xSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
  ySensor.set_CS_AutocaL_Millis(0xFFFFFFFF);
  zSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);

  pinMode(ARDUINO_COMM, OUTPUT);
  analogWrite(ARDUINO_COMM, 0);

  Serial.begin(9600);
}

long prevX;
long prevY;
long prevZ;

void loop() {
  // put your main code here, to run repeatedly:
  long x = xSensor.capacitiveSensor(300);
  long y = ySensor.capacitiveSensor(300);
  long z = zSensor.capacitiveSensor(300);

  long xDiff = x - prevX;
  long yDiff = y - prevY;
  long zDiff = z - prevZ;

  if(max(max(abs(xDiff), abs(yDiff)), abs(zDiff)) > 30){
    if(abs(xDiff) > max(abs(yDiff), abs(zDiff))){
      if(xDiff > 0)
        analogWrite(ARDUINO_COMM, RIGHT);
      else
        analogWrite(ARDUINO_COMM, LEFT);
    } else if (abs(yDiff) > max(abs(xDiff), abs(zDiff))) {
      if(yDiff > 0)
        analogWrite(ARDUINO_COMM, UP);
      else
        analogWrite(ARDUINO_COMM, DOWN);
    } else {
      if(zDiff > 0)
        analogWrite(ARDUINO_COMM, FORWARD);
      else
        analogWrite(ARDUINO_COMM, BACK);
    }
  }
  
  prevX = x;
  prevY = y;
  prevZ = z;
  delay(100);
}
