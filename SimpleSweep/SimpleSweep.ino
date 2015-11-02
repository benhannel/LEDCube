uint8_t P_PINS[8] = {13, 12, A4, A3, A0, 11, A2, A1};
uint8_t N_PINS[8] = {3, 2, 4, 5, 7, 6, 8, 9};
int NUM_PINS = 8;

void setup() {
  // initialize all the pins as outputs initially set to
  // HIGH, ie off.
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    pinMode(P_PINS[i], OUTPUT);
    digitalWrite(P_PINS[i], HIGH);
    pinMode(N_PINS[i], OUTPUT);
    digitalWrite(N_PINS[i], HIGH);
  }
}

int loopNum = 0;

void loop() {
  // scan throught the "rows" by turning the anodes of a row
  // of LEDs on one at a time
  for (uint8_t anum = 0; anum < NUM_PINS; anum++) {
    digitalWrite(P_PINS[anum], LOW);
    // within each row, blink each LED on one at a time by
    // turning on the cathodes/columns
    for (uint8_t cnum = 0; cnum < NUM_PINS; cnum++) {
      digitalWrite(N_PINS[cnum], LOW);
      delay(300);
      // remember to clean up when you're done
      digitalWrite(N_PINS[cnum], HIGH);
    }
    // remember to clean up when you're done
    digitalWrite(P_PINS[anum], HIGH);
  }
}

