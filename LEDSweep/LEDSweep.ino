uint8_t P_PINS[8] = {13, 12, A4, A3, A0, 11, A2, A1};
uint8_t N_PINS[8] = {3, 2, 4, 5, 7, 6, 8, 9};
int NUM_PINS = 8;
int SIDE_LENGTH = 4;

void setup() {
  // put your setup code here, to run once:
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    pinMode(P_PINS[i], OUTPUT);
    digitalWrite(P_PINS[i], HIGH);
    pinMode(N_PINS[i], OUTPUT);
    digitalWrite(N_PINS[i], HIGH);
  }
  
  Serial.begin(9600);
}

int loopNum = 0;

void loop() {
  // put your main code here, to run repeatedly:
  loopNum++;

 uint8_t values[4][4][4];
 for (uint8_t x = 0; x < SIDE_LENGTH; x++) {
   for (uint8_t y = 0; y < SIDE_LENGTH; y++) {
     for (uint8_t z = 0; z < SIDE_LENGTH; z++) {
       values[x][y][z] = 0;
     }
   }
 }
 
 for (uint8_t x = 0; x < SIDE_LENGTH; x++) {
   for (uint8_t z = 0; z < SIDE_LENGTH; z++) {
     values[x][0][z] = 16; //(x + y + z + loopNum / 10) % 16;
     for(uint8_t i = 0; i < 50; i++){
       display(values);
     }
     values[x][0][z] = 0;
   }
 }
}

/*
 * Turns on all the LEDs specified in values over 80 milliseconds.
 */
void display(uint8_t values[4][4][4]) {
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    digitalWrite(P_PINS[i], LOW);
    for (uint8_t fractional_time = 0; fractional_time <= 16; fractional_time++){
      for (uint8_t j = 0; j < NUM_PINS; j++) {
        digitalWrite(N_PINS[j], getValue(values, i, j) <= fractional_time);
      }
      delayMicroseconds(10);
    }
    digitalWrite(P_PINS[i], HIGH);
  }
}

uint8_t getValue(uint8_t values[4][4][4], uint8_t negativePin, uint8_t positivePin) {
  uint8_t layer;
  if(positivePin > 3){
    if(negativePin > 3)
      layer = 2;
    else
      layer = 1;
  } else {
    if(negativePin > 3)
      layer = 3;
    else
      layer = 0;
  }
  uint8_t x = negativePin % 4;
  uint8_t y = positivePin % 4;
  return values[x][y][layer];
}

